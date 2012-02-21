/*
 *	File and directory handling
 *	Copyright © Jan Engelhardt <jengelh [at] medozas de>, 2002-2011
 *
 *	This file is part of libHX. libHX is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2.1 or 3 of the License.
 */
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined _WIN32
#	include <direct.h>
#	include <io.h>
#	include <windows.h>
#else
#	include <dirent.h>
#	include <unistd.h>
#endif
#include <libHX/ctype_helper.h>
#include <libHX/io.h>
#include <libHX/misc.h>
#include <libHX/string.h>
#include "internal.h"

struct HXdir {
#if defined _WIN32
	char *dname;
	HANDLE ptr;
	WIN32_FIND_DATA dentry;
	bool got_first;
#else
	DIR *ptr;
	/*
	 * Surprise surprise, along comes Solaris with a dirent too short!
	 */
	union {
		struct dirent dentry;
		char extender[_POSIX_PATH_MAX + sizeof(struct dirent) - 
			HXsizeof_member(struct dirent, d_name)];
	};
#endif
};

static int mkdir_gen(const char *d)
{
	struct stat sb;
	if (lstat(d, &sb) < 0) {
#if defined(_WIN32)
		if (mkdir(d) < 0)
#else
		if (mkdir(d, 0777) < 0) /* use umask() for permissions */
#endif
			return -errno;
	} else {
#if defined(_WIN32)
		if ((sb.st_mode & S_IFDIR) != S_IFDIR)
#else
		if (!S_ISDIR(sb.st_mode))
#endif
			return -errno;
	}
	return 1;
}

EXPORT_SYMBOL struct HXdir *HXdir_open(const char *s)
{
	struct HXdir *d;
	if ((d = malloc(sizeof(struct HXdir))) == NULL)
		return NULL;

#if defined(_WIN32)
	if ((d->dname = malloc(strlen(s) + 3)) == NULL)
		goto out;
	strcpy(d->dname, s);
	strcat(d->dname, "\\*");
	d->got_first = false;
#else
	if ((d->ptr = opendir(s)) == NULL)
		goto out;
#endif

	return d;
 out:
	free(d);
	return NULL;
}

EXPORT_SYMBOL const char *HXdir_read(struct HXdir *d)
{
	if (d == NULL)
		return NULL;

	errno = 0;
#if defined _WIN32
	if (!d->got_first) {
		d->got_first = true;
		if ((d->ptr = FindFirstFile(d->dname, &d->dentry)) == NULL)
			return NULL;
	} else if (!FindNextFile(d->ptr, &d->dentry)) {
		return NULL;
	}
	return d->dentry.cFileName;
#else
	{
		struct dirent *checkptr;
		int i = readdir_r(d->ptr, &d->dentry, &checkptr);
		if (checkptr == NULL || i < 0)
			return NULL;
	}
	return d->dentry.d_name;
#endif
}

EXPORT_SYMBOL void HXdir_close(struct HXdir *d)
{
	if (d == NULL)
		return;
#if defined _WIN32
	FindClose(d->ptr);
	free(d->dname);
#else
	closedir(d->ptr);
#endif
	free(d);
}

EXPORT_SYMBOL int HX_copy_file(const char *src, const char *dest,
    unsigned int opts, ...)
{
	char buf[MAXLNLEN];
	unsigned int extra_flags = 0;
	int dd, eax = 0, sd, l;
	int throw_away;

	if ((sd = open(src, O_RDONLY | O_BINARY)) < 0)
		return -errno;
	if (opts & HXF_KEEP)
		extra_flags = O_EXCL;
	dd = open(dest, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC |
	     extra_flags, S_IRUGO | S_IWUGO);
	if (dd < 0) {
		eax = errno;
		close(sd);
		errno = eax;
		if (extra_flags != 0 && eax == EEXIST)
			return 1;
		return -errno;
	}

	while ((l = read(sd, buf, MAXLNLEN)) > 0 && write(dd, buf, l) > 0)
		;
	close(sd);

	if (opts & (HXF_UID | HXF_GID)) {
		struct stat sb;
		long uid, gid;
		va_list argp;
		va_start(argp, opts);

		fstat(dd, &sb);
		uid = sb.st_uid;
		gid = sb.st_gid;

		if (opts & HXF_UID) uid = va_arg(argp, long);
		if (opts & HXF_GID) gid = va_arg(argp, long);
		throw_away = fchown(dd, uid, gid);
		va_end(argp);
	}
	close(dd);
	return 1;
}

EXPORT_SYMBOL int HX_copy_dir(const char *src, const char *dest,
    unsigned int opts, ...)
{
	void *dt = HXdir_open(src);
	long uid = -1, gid = -1;
	const char *fn;
	int throw_away;

	if (dt == NULL)
		return 0;

	{
		va_list argp;
		va_start(argp, opts);
		if (opts & HXF_UID) uid = va_arg(argp, long);
		if (opts & HXF_GID) gid = va_arg(argp, long);
		va_end(argp);
	}

	while ((fn = HXdir_read(dt)) != NULL) {
		char fsrc[MAXFNLEN], fdest[MAXFNLEN];
		struct stat sb;

		if (strcmp(fn, ".") == 0 || strcmp(fn, "..") == 0)
			continue;
		snprintf(fsrc,  MAXFNLEN, "%s/%s", src,  fn);
		snprintf(fdest, MAXFNLEN, "%s/%s", dest, fn);

		lstat(fsrc, &sb);
		sb.st_mode &= 0777; /* clear SUID/GUID/Sticky bits */

		if (S_ISREG(sb.st_mode)) {
			HX_copy_file(fsrc, fdest, opts, uid, gid);
		} else if (S_ISDIR(sb.st_mode)) {
			HX_mkdir(fdest);
			HX_copy_dir(fsrc, fdest, opts, uid, gid);
		} else if (S_ISLNK(sb.st_mode)) {
			char pt[MAXFNLEN];
			memset(pt, '\0', MAXFNLEN);
			if (readlink(fsrc, pt, MAXFNLEN - 1) < MAXFNLEN - 1)
				throw_away = symlink(pt, fdest);
		} else if (S_ISBLK(sb.st_mode) || S_ISCHR(sb.st_mode)) {
			mknod(fdest, sb.st_mode, sb.st_dev);
		} else if (S_ISFIFO(sb.st_mode)) {
			mkfifo(fdest, sb.st_mode);
		}

		throw_away = lchown(fdest, uid, gid);
		if (!S_ISLNK(sb.st_mode))
			chmod(fdest, sb.st_mode);
	}

	HXdir_close(dt);
	return 1;
}

EXPORT_SYMBOL int HX_mkdir(const char *idir)
{
	int i = 0, len = strlen(idir);
	char buf[MAXFNLEN], dir[MAXFNLEN];

#if defined(_WIN32)
	{
		char *p = dir;
		HX_strlcpy(dir, idir, sizeof(dir));
		for (; *p != '\0'; ++p)
			if (*p == '\\')
				*p = '/';
		if (HX_isalpha(dir[0]) && dir[1] == ':')
			i = 2;
	}
#else
	HX_strlcpy(dir, idir, sizeof(dir));
#endif

	if (dir[i] == '/')
		++i;
	for (; i < len; ++i) {
		int v;
		if (dir[i] == '/') {
			strncpy(buf, dir, i);
			buf[i] = '\0';
			if ((v = mkdir_gen(buf)) <= 0)
				return v;
		} else if (i == len - 1) {
			strncpy(buf, dir, len);
			buf[len] = '\0';
			if ((v = mkdir_gen(buf)) <= 0)
				return v;
		}
	}
	return 1;
}

/* Readlink - with a trailing zero (provided by HXmc) */
EXPORT_SYMBOL int HX_readlink(hxmc_t **target, const char *path)
{
	bool dnull = *target == NULL;
	char *tb;
	int ret;

	if (dnull) {
		*target = HXmc_meminit(NULL, PATH_MAX);
		if (*target == NULL)
			return -errno;
	}
	tb  = *target;
	ret = readlink(path, tb, PATH_MAX);
	if (ret < 0) {
		ret = -errno;
		if (!dnull) {
			HXmc_free(*target);
			*target = NULL;
		}
		return ret;
	}
	HXmc_setlen(target, ret);
	return ret;
}

/**
 * The buffers HX_realpath_symres are used are retained across symres calls to
 * not do unnecessarily many allocation calls. Downside is that the state is
 * roughly 12K in the worst case.
 */
struct HX_realpath_state {
	hxmc_t *dest;
	hxmc_t *link_target;
	hxmc_t *new_path;
	hxmc_t *symres_tmp;
	const char *path;
	unsigned int deref_count;
};

/**
 * Perform symlink resolution on the currently last component (state->dest).
 */
static int HX_realpath_symres(struct HX_realpath_state *state, const char *path)
{
	int ret;

	ret = HX_readlink(&state->link_target, state->dest);
	if (ret == -EINVAL)
		return -EINVAL;
	else if (ret < 0)
		return -errno;
	else if (state->deref_count++ >= 40)
		return -ELOOP;

	if (*state->link_target == '/') {
		*state->dest = '\0';
		if (HXmc_setlen(&state->dest, 0) == NULL)
			return -errno;
	} else {
		char *dptr = state->dest + HXmc_length(state->dest);
		while (*--dptr != '/')
			;
		*dptr = '\0';
		if (HXmc_setlen(&state->dest, dptr - state->dest) == NULL)
			return -errno;
	}

	if (HXmc_strcpy(&state->symres_tmp, state->link_target) == NULL)
		return -errno;
	/*
	 * @path could be pointing to @state->new_path already, so we need
	 * to construct the new path in a temp buffer (@symres_tmp) first.
	 */
	if (HXmc_strcat(&state->symres_tmp, path) == NULL)
		return -errno;
	if (HXmc_strcpy(&state->new_path, state->symres_tmp) == NULL)
		return -errno;
	state->path = state->new_path;
	return 1;
}

EXPORT_SYMBOL int HX_realpath(hxmc_t **dest_pptr, const char *path,
    unsigned int flags)
{
	struct HX_realpath_state state = {.dest = *dest_pptr};
	bool rq_slash = false, dnull = state.dest == NULL;
	const char *cptr, *orig_path = path;
	int ret = 0;

	if (dnull) {
		state.dest = HXmc_meminit(NULL, PATH_MAX);
		if (state.dest == NULL)
			goto err;
	}

	if (*path == '/') {
		rq_slash = true;
	} else if (flags & HX_REALPATH_ABSOLUTE) {
		if (getcwd(state.dest, PATH_MAX) == NULL)
			goto err;
		rq_slash = true;
		if (HXmc_setlen(&state.dest, strlen(state.dest)) == NULL)
			goto err;
	}

	while (*path != '\0') {
		if (*path == '/') {
			++path;
			continue;
		} else if (path[0] == '.' &&
		    (path[1] == '/' || path[1] == '\0') &&
		    flags & HX_REALPATH_SELF) {
			++path;
			continue;
		} else if (path[0] == '.' && path[1] == '.' &&
		    (path[2] == '/' || path[2] == '\0') &&
		    flags & HX_REALPATH_PARENT &&
		    ((flags & HX_REALPATH_ABSOLUTE) || *state.dest != '\0')) {
			cptr  = state.dest + HXmc_length(state.dest);
			path += 2;
			while (cptr > state.dest && *--cptr != '/')
				;
			state.dest[cptr-state.dest] = '\0';
			if (HXmc_setlen(&state.dest,
			    cptr - state.dest) == NULL)
				goto err;
			continue;
		}

		for (cptr = path; *cptr != '\0' && *cptr != '/'; ++cptr)
			;
		if (rq_slash && HXmc_strcat(&state.dest, "/") == NULL)
			goto out;
		if (HXmc_memcat(&state.dest, path, cptr - path) == NULL)
			goto out;
		path = cptr;
		rq_slash = true;

		if (!(flags & HX_REALPATH_SYMLINK))
			continue;
		ret = HX_realpath_symres(&state, path);
		if (ret == -EINVAL)
			continue;
		else if (ret < 0)
			goto out;
		path = state.path;
	}

	if (*state.dest == '\0') {
		if (*orig_path == '/') {
			if (HXmc_strcpy(&state.dest, "/") == NULL)
				goto err;
		} else {
			if (HXmc_strcpy(&state.dest, ".") == NULL)
				goto err;
		}
	}

	*dest_pptr = state.dest;
	HXmc_free(state.link_target);
	HXmc_free(state.new_path);
	HXmc_free(state.symres_tmp);
	return 1;

 err:
	ret = -errno;
 out:
	if (dnull) {
		/* If caller supplied a buffer, do not take it away. */
		HXmc_free(state.dest);
		*dest_pptr = NULL;
	}
	HXmc_free(state.link_target);
	HXmc_free(state.new_path);
	HXmc_free(state.symres_tmp);
	return ret;
}

EXPORT_SYMBOL int HX_rrmdir(const char *dir)
{
	struct HXdir *ptr;
	const char *trav;
	hxmc_t *fn = NULL;
	int ret = 0;

	if ((ptr = HXdir_open(dir)) == NULL)
		return -errno;

	while ((trav = HXdir_read(ptr)) != NULL) {
		struct stat sb;

		if (strcmp(trav, ".") == 0 || strcmp(trav, "..") == 0)
			continue;
		HXmc_strcpy(&fn, dir);
		HXmc_strcat(&fn, "/");
		HXmc_strcat(&fn, trav);
		if (lstat(fn, &sb) < 0) {
			if (ret == 0)
				ret = -errno;
			continue;
		}

		if (S_ISDIR(sb.st_mode)) {
			if (HX_rrmdir(fn) <= 0) {
				if (ret == 0)
					ret = -errno;
				continue;
			}
		} else if (unlink(fn) < 0) {
			if (ret == 0)
				ret = -errno;
			continue;
		}
	}

	if (rmdir(dir) < 0) {
		if (ret == 0)
			ret = -errno;
	}
	HXdir_close(ptr);
	HXmc_free(fn);
	return ret;
}

EXPORT_SYMBOL ssize_t HXio_fullread(int fd, void *buf, size_t size)
{
	size_t rem = size;
	ssize_t ret;

	while (rem > 0) {
		ret = read(fd, buf, rem);
		if (ret < 0)
			return ret;
		rem -= ret;
		buf += ret;
	}
	return size;
}

EXPORT_SYMBOL ssize_t HXio_fullwrite(int fd, const void *buf, size_t size)
{
	size_t rem = size;
	ssize_t ret;

	while (rem > 0) {
		ret = write(fd, buf, rem);
		if (ret < 0)
			return ret;
		rem -= ret;
		buf += ret;
	}
	return size;
}
