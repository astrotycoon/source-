#include <errno.h>
#include <stddef.h>
#include "internal.h"

EXPORT_SYMBOL int chown(const char *path, long uid, long gid)
{
	return -(errno = ENOSYS);
}

EXPORT_SYMBOL int fchmod(int fd, long perm)
{
	return -(errno = ENOSYS);
}

EXPORT_SYMBOL int fchown(int fd, long uid, long gid)
{
	return -(errno = ENOSYS);
}

EXPORT_SYMBOL int lchown(const char *path, long uid, long gid)
{
	return -(errno = ENOSYS);
}

EXPORT_SYMBOL int lstat(const char *path, struct stat *sb)
{
	return stat(path, sb);
}

EXPORT_SYMBOL int mkfifo(const char *path, long mode)
{
	return -(errno = EPERM);
}

EXPORT_SYMBOL int mknod(const char *path, long mode, long dev)
{
	return -(errno = EPERM);
}

EXPORT_SYMBOL int readlink(const char *path, char *dest, size_t len)
{
	return -(errno = EINVAL);
}

EXPORT_SYMBOL int symlink(const char *src, const char *dest)
{
	return -(errno = EPERM);
}
