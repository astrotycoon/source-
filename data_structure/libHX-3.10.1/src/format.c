/*
 *	String placeholder expansion
 *	Copyright © Jan Engelhardt <jengelh [at] medozas de>, 2007 - 2010
 *
 *	This file is part of libHX. libHX is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2.1 or 3 of the License.
 */
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libHX.h>
#include "internal.h"

/* Definitions */
#define MAX_KEY_SIZE 256
/* To make it easier on the highlighter */
#define C_OPEN  '('
#define C_CLOSE ')'
#define S_OPEN  "("
#define S_CLOSE ")"

struct fmt_entry {
	const void *ptr;
	unsigned int type;
};

static inline struct HXformat_map *fmt_export(const struct HXmap *t)
{
	return const_cast1(void *, static_cast(const void *, t));
}

static inline struct HXmap *fmt_import(const struct HXformat_map *t)
{
	return const_cast1(void *, static_cast(const void *, t));
}

static void fmt_entry_free(void *e)
{
	struct fmt_entry *entry = e;

	if (entry->type == (HXTYPE_STRING | HXFORMAT_IMMED))
		free(const_cast1(void *, entry->ptr));
	free(entry);
}

static const struct HXmap_ops fmt_entry_ops = {
	.d_free = fmt_entry_free,
};

EXPORT_SYMBOL struct HXformat_map *HXformat_init(void)
{
	struct HXmap *table;

	table = HXmap_init5(HXMAPT_DEFAULT, HXMAP_SCKEY, &fmt_entry_ops,
	        0, sizeof(struct fmt_entry));
	if (table == NULL)
		return NULL;
	return fmt_export(table);
}

EXPORT_SYMBOL void HXformat_free(struct HXformat_map *ftable)
{
	HXmap_free(fmt_import(ftable));
}

EXPORT_SYMBOL int HXformat_add(struct HXformat_map *ftable, const char *key,
    const void *ptr, unsigned int ptr_type)
{
	struct HXmap *table = fmt_import(ftable);
	struct fmt_entry *entry;
	int ret;

	if (strpbrk(key, "\t\n\v ") != NULL || strlen(key) > MAX_KEY_SIZE ||
	    *key == '\0') {
		fprintf(stderr, "%s: Bogus key \"%s\"\n", __func__, key);
		return -EINVAL;
	}

	if ((entry = malloc(sizeof(*entry))) == NULL)
		return -errno;

	entry->type = ptr_type;
	if (ptr_type == (HXTYPE_STRING | HXFORMAT_IMMED)) {
		if ((entry->ptr = HX_strdup(ptr)) == NULL) {
			free(entry);
			return -errno;
		}
	} else {
		entry->ptr = ptr;
	}

	ret = HXmap_add(table, key, entry);
	if (ret <= 0) {
		free(entry);
		return ret;
	}

	return 1;
}

static inline char *HX_strchr0(const char *s, char c)
{
	char *ret = strchr(s, c);
	if (ret != NULL)
		return ret;
	return const_cast1(char *, &s[strlen(s)]);
}

/*
 *	HXformat2
 *
 *	Compared to the first-generation, this variant provides make(1)-style
 *	function calls.
 */

struct HXformat2_fd {
	const char *name;
	hxmc_t *(*proc)(int, const char *const *);
	const char *delim;
	bool (*check)(const struct HXmap *);
};

/*
 * Used as an unique object for "expanded to nothing", to distinguish it from
 * %NULL indicating some error. This is cheaper than allocating an empty hxmc
 * everytime.
 */
static char HXformat2_nexp;

static inline void HXformat2_insuf(const char *func, int argc)
{
	fprintf(stderr, "%s: insufficient number of arguments (%d)\n",
	        func, argc);
}

/*
 * Echo input back, with markers. This is strictly for testing only.
 */
static hxmc_t *HXformat2_echo(int argc, const hxmc_t *const *argv)
{
	hxmc_t *ret = HXmc_meminit(NULL, 0);
	int i;

	HXmc_strcat(&ret, "<echo");
	for (i = 0; i < argc; ++i) {
		HXmc_strcat(&ret, " [");
		HXmc_strcat(&ret, argv[i]);
		HXmc_strcat(&ret, "]");
	}
	HXmc_strcat(&ret, ">");
	return ret;
}

static hxmc_t *HXformat2_env(int argc, const hxmc_t *const *argv)
{
	const char *s;

	if (argc == 0)
		return &HXformat2_nexp;
	s = getenv(argv[0]);
	return (s == NULL) ? &HXformat2_nexp : HXmc_strinit(s);
}

static hxmc_t *HXformat2_if(int argc, const hxmc_t *const *argv)
{
	if (argc < 2) {
		HXformat2_insuf(__func__, argc);
		return &HXformat2_nexp;
	}

	if (*argv[0] != '\0')
		return (*argv[1] != '\0') ?
		       HXmc_strinit(argv[1]) : &HXformat2_nexp;

	return (argc >= 3 && *argv[2] != '\0') ?
	       HXmc_strinit(argv[2]) : &HXformat2_nexp;
}

static hxmc_t *HXformat2_lower(int argc, const hxmc_t *const *argv)
{
	hxmc_t *ret;

	if (argc == 0)
		return &HXformat2_nexp;
	ret = HXmc_strinit(argv[0]);
	HX_strlower(ret);
	return ret;
}

static bool HXformat2_execchk(const struct HXmap *table)
{
	return HXmap_find(table, "/libhx/exec") != NULL;
}

static hxmc_t *HXformat2_exec1(const hxmc_t *const *argv, bool shell)
{
	struct HXproc proc = {
		.p_flags = HXPROC_NULL_STDIN | HXPROC_STDOUT | HXPROC_VERBOSE,
	};
	hxmc_t *slurp, *complete = NULL;
	ssize_t ret;

	slurp = HXmc_meminit(NULL, BUFSIZ);
	if (slurp == NULL)
		return NULL;
	complete = HXmc_meminit(NULL, BUFSIZ);
	if (complete == NULL)
		goto out;

	ret = HXproc_run_async(argv, &proc);
	if (ret < 0)
		goto out;
	while ((ret = read(proc.p_stdout, slurp, BUFSIZ)) > 0)
		if (HXmc_memcat(&complete, slurp, ret) == NULL)
			break;
	close(proc.p_stdout);
	HXproc_wait(&proc);
	HXmc_free(slurp);
	return complete;
 out:
	HXmc_free(complete);
	HXmc_free(slurp);
	return &HXformat2_nexp;
}

static hxmc_t *HXformat2_exec(int argc, const hxmc_t *const *argv)
{
	if (argc == 0)
		return &HXformat2_nexp;
	return HXformat2_exec1(argv, false);
}

static hxmc_t *HXformat2_shell(int argc, const hxmc_t *const *argv)
{
	const char *cmd[] = {"/bin/sh", "-c", NULL, NULL};
	if (argc == 0)
		return &HXformat2_nexp;
	cmd[2] = argv[0];
	return HXformat2_exec1(cmd, true);
}

static hxmc_t *HXformat2_snl(int argc, const hxmc_t *const *argv)
{
	hxmc_t *s;
	char *p;

	if (argc == 0)
		return &HXformat2_nexp;
	p = s = HXmc_strinit(*argv);
	if (s == NULL)
		return NULL;
	HX_chomp(s);
	while ((p = strchr(p, '\n')) != NULL)
		*p++ = ' ';
	return s;
}

static hxmc_t *HXformat2_substr(int argc, const hxmc_t *const *argv)
{
	ssize_t offset, length, z;
	hxmc_t *ret;
	char *end;

	if (argc < 2) {
		HXformat2_insuf(__func__, argc);
		return &HXformat2_nexp;
	}

	offset = strtoll(argv[1], &end, 0);
	if (*end != '\0') {
		fprintf(stderr, "HXformat2-substr: found garbage in "
		        "offset specification\n");
		return &HXformat2_nexp;
	}

	z = strlen(argv[0]);
	if (offset < 0)
		offset = z + offset;
	if (offset >= z)
		return &HXformat2_nexp;

	if (argc == 2) {
		if (offset < 0)
			offset = 0;
		length = z - offset;
	} else {
		length = strtoll(argv[2], &end, 0);
		if (*end != '\0') {
			fprintf(stderr, "HXformat2-substr; found garbage in "
			        "length specification\n");
			return &HXformat2_nexp;
		}
		if (length < 0)
			length/*end*/ = z + length;
		else
			length/*end*/ = offset + length;
		if (offset < 0)
			offset = 0;
	}
	if (length <= 0)
		return &HXformat2_nexp;

	ret = HXmc_meminit(NULL, length);
	if (ret == NULL)
		return &HXformat2_nexp;
	if (HXmc_memcpy(&ret, &argv[0][offset], length) == NULL) {
		HXmc_free(ret);
		return &HXformat2_nexp;
	}
	return ret;
}

static hxmc_t *HXformat2_upper(int argc, const hxmc_t *const *argv)
{
	hxmc_t *ret;

	if (argc == 0)
		return &HXformat2_nexp;
	ret = HXmc_strinit(argv[0]);
	HX_strupper(ret);
	return ret;
}

static const struct HXformat2_fd HXformat2_fmap[] = {
	/* Need to be alphabetically sorted */
	{"echo",	HXformat2_echo,		S_CLOSE " ,"},
	{"env",		HXformat2_env,		S_CLOSE " ,"},
	{"exec",	HXformat2_exec,		S_CLOSE " ", HXformat2_execchk},
	{"if",		HXformat2_if,		S_CLOSE ","}, /* no sp: ok */
	{"lower",	HXformat2_lower,	S_CLOSE},
	{"shell",	HXformat2_shell,	S_CLOSE, HXformat2_execchk},
	{"snl",		HXformat2_snl,		S_CLOSE},
	{"substr",	HXformat2_substr,	S_CLOSE ","},
	{"upper",	HXformat2_upper,	S_CLOSE},
};

static int HXformat2_fmap_compare(const void *pa, const void *pb)
{
	const char *a_name = pa;
	const struct HXformat2_fd *b = pb;
	return strcmp(a_name, b->name);
}

/**
 * HXformat2_xcall - expand function call (gather args)
 * @name:	name of function
 * @pptr:	pointer to position in string
 * @table:	table of known variables
 *
 * @*pptr must point to the first character of the first argument to the
 * function.
 */
static hxmc_t *HXformat2_xcall(const char *name, const char **pptr,
    const struct HXmap *table)
{
	const struct HXformat2_fd *entry;
	hxmc_t *ret, *ret2, **argv;
	struct HXdeque *dq;
	const char *s, *delim;
	int err;

	dq = HXdeque_init();
	if (dq == NULL)
		return NULL;

	entry = bsearch(name, HXformat2_fmap, ARRAY_SIZE(HXformat2_fmap),
	        sizeof(*HXformat2_fmap), HXformat2_fmap_compare);
	delim = (entry != NULL) ? entry->delim : S_CLOSE;
	if (**pptr == C_CLOSE)
		++*pptr;
	else for (s = *pptr; *s != '\0'; s = ++*pptr) {
		while (HX_isspace(*s))
			++s;
		*pptr = s;
		ret = HXparse_dequote_fmt(s, delim, pptr);
		if (ret == NULL)
			goto out;
		if (strstr(ret, "%" S_OPEN) != NULL) {
			ret2 = NULL;
			err = HXformat2_aprintf(fmt_export(table), &ret2, ret);
			if (ret2 == NULL)
				goto out2;
			HXmc_free(ret);
			ret = ret2;
		}
		if (HXdeque_push(dq, ret) == NULL)
			goto out2;
		if (**pptr == '\0')
			break;
		if (**pptr == C_CLOSE) {
			++*pptr;
			break;
		}
	}

	ret = NULL;
	argv = reinterpret_cast(hxmc_t **, HXdeque_to_vec(dq, NULL));
	if (argv == NULL)
		goto out;

	ret = &HXformat2_nexp;
	/* Unknown functions are silently expanded to nothing, like make. */
	if (entry != NULL && (entry->check == NULL || entry->check(table)))
		ret = entry->proc(dq->items,
		      const_cast2(const hxmc_t *const *, argv));
	/*
	 * Pointers in argv are shared with those in dq.
	 * Free only the outer shell of one.
	 */
	free(argv);
 out:
	HXdeque_genocide2(dq, static_cast(void *, HXmc_free));
	return ret;
 out2:
	HXmc_free(ret);
	ret = NULL;
	goto out;
}

/**
 * HXformat2_xvar - expand a variable
 * @entry:	the variable to expand
 */
static hxmc_t *HXformat2_xvar(const struct fmt_entry *entry)
{
#define IMM(fmt, type) \
	snprintf(buf, sizeof(buf), (fmt), \
		static_cast(type, reinterpret_cast(long, entry->ptr))); \
	break;
#define PTR(fmt, type) \
	snprintf(buf, sizeof(buf), (fmt), \
		*static_cast(const type *, entry->ptr)); \
	break;

	static const char *const tf[] = {"false", "true"};
	char buf[HXSIZEOF_Z64];
	hxmc_t *wp = NULL;

	*buf = '\0';
	switch (entry->type) {
		case HXTYPE_STRING:
		case HXTYPE_STRING | HXFORMAT_IMMED:
			HXmc_strcpy(&wp, entry->ptr);
			break;
		case HXTYPE_STRP:
			HXmc_strcpy(&wp, *static_cast(const char *const *, entry->ptr));
			break;
		case HXTYPE_BOOL:
			HXmc_strcpy(&wp, tf[!!*static_cast(const int *,
			           entry->ptr)]);
			break;
		case HXTYPE_BOOL | HXFORMAT_IMMED:
			HXmc_strcpy(&wp, tf[entry->ptr != NULL]);
			break;

		case HXTYPE_BYTE:   PTR("%c", unsigned char);
		case HXTYPE_SHORT:  PTR("%hd", short);
		case HXTYPE_USHORT: PTR("%hu", unsigned short);
		case HXTYPE_CHAR:   PTR("%d", char);
		case HXTYPE_UCHAR:  PTR("%u", unsigned char);
		case HXTYPE_INT:    PTR("%d", int);
		case HXTYPE_UINT:   PTR("%u", unsigned int);
		case HXTYPE_LONG:   PTR("%ld", long);
		case HXTYPE_ULONG:  PTR("%lu", unsigned long);
		case HXTYPE_LLONG:  PTR("%lld", long long);
		case HXTYPE_ULLONG: PTR("%llu", unsigned long long);
		case HXTYPE_FLOAT:  PTR("%f", float);
		case HXTYPE_DOUBLE: PTR("%f", double);

		case HXTYPE_CHAR   | HXFORMAT_IMMED: IMM("%d", char);
		case HXTYPE_UCHAR  | HXFORMAT_IMMED: IMM("%u", unsigned char);
		case HXTYPE_SHORT  | HXFORMAT_IMMED: IMM("%hd", short);
		case HXTYPE_USHORT | HXFORMAT_IMMED: IMM("%hu", unsigned short);
		case HXTYPE_INT    | HXFORMAT_IMMED: IMM("%d", int);
		case HXTYPE_UINT   | HXFORMAT_IMMED: IMM("%u", unsigned int);
		case HXTYPE_LONG   | HXFORMAT_IMMED: IMM("%ld", long);
		case HXTYPE_ULONG  | HXFORMAT_IMMED: IMM("%lu", unsigned long);

		default:
			fprintf(stderr, "%s: Illegal type\n", __func__);
			return &HXformat2_nexp;
	}

	if (*buf != '\0')
		HXmc_strcpy(&wp, buf);
	return wp;
#undef IMM
#undef PTR
}

/**
 * HXformat2_xany - expand function call or variable
 * @pptr:	pointer to position in string
 * @table:	map of available variables
 *
 * @*pptr has to point to the first character after the "%(" opener.
 */
static hxmc_t *HXformat2_xany(const char **pptr, const struct HXmap *table)
{
	const char *s = *pptr;
	hxmc_t *name, *ret;

	/*
	 * Some shortcuts for cases that always expand to nothing.
	 * %() and %( ).
	 */
	if (*s == C_CLOSE) {
		++*pptr;
		return &HXformat2_nexp;
	} else if (HX_isspace(*s)) {
		while (HX_isspace(*s))
			++s;
		HXmc_free(HXparse_dequote_fmt(s, S_CLOSE, pptr));
		++*pptr;
		return &HXformat2_nexp;
	}

	/* Long parsing */
	name = HXparse_dequote_fmt(s, S_CLOSE " \t\n\f\v\r", pptr);
	if (name == NULL)
		return NULL;
	s = *pptr;
	if (*s == '\0') {
		fprintf(stderr, "libHX-format2: "
		        "unterminated variable reference / "
		        "missing closing parenthesis.\n");
		return NULL;
	} else if (*s == C_CLOSE) {
		/* Closing parenthesis - variable */
		const struct fmt_entry *entry;
		hxmc_t *new_name = NULL;
		int eret;

		*pptr = ++s;
		eret  = HXformat2_aprintf(fmt_export(table), &new_name, name);
		if (eret <= 0) {
			ret = NULL;
		} else if (*new_name == '\0') {
			ret = &HXformat2_nexp;
		} else {
			entry = HXmap_get(table, new_name);
			ret   = (entry == NULL) ? &HXformat2_nexp :
			        HXformat2_xvar(entry);
		}
		HXmc_free(new_name);
	} else {
		/* Seen whitespace - function */
		while (HX_isspace(*s))
			++s;
		*pptr = s;
		/*
		 * Note that %() is not expanded in function names. This
		 * follows make(1) behavior.
		 */
		ret = HXformat2_xcall(name, pptr, table);
	}

	HXmc_free(name);
	return ret;
}

EXPORT_SYMBOL int HXformat2_aprintf(const struct HXformat_map *ftable,
    hxmc_t **resultp, const char *fmt)
{
	const struct HXmap *table = fmt_import(ftable);
	hxmc_t *ex, *ts, *out = HXmc_meminit(NULL, 0);
	const char *current;
	int ret = 0;

	current = fmt;
	while ((current = HX_strchr0(fmt, '%')) != NULL) {
		if (current - fmt > 0)
			HXmc_memcat(&out, fmt, current - fmt);
		if (*current == '\0')
			break;
		if (current[1] != C_OPEN) {
			HXmc_memcat(&out, current, 2);
			fmt = current + 2;
			continue;
		}

		current += 2; /* skip % and opening parenthesis */
		ex = HXformat2_xany(&current, table);
		if (ex == NULL)
			goto out;
		if (ex != &HXformat2_nexp) {
			ts = HXmc_strcat(&out, ex);
			HXmc_free(ex);
			if (ts == NULL)
				goto out;
		}
		fmt = current;
	}

	*resultp = out;
	return strlen(out);

 out:
	ret = -errno;
	HXmc_free(out);
	return ret;
}

EXPORT_SYMBOL int HXformat2_fprintf(const struct HXformat_map *ftable,
    FILE *filp, const char *fmt)
{
	hxmc_t *str;
	int ret;

	if ((ret = HXformat2_aprintf(ftable, &str, fmt)) <= 0)
		return ret;
	errno = 0;
	if (fputs(str, filp) < 0)
		ret = -errno;
	HXmc_free(str);
	return ret;
}

EXPORT_SYMBOL int HXformat2_sprintf(const struct HXformat_map *ftable,
    char *dest, size_t size, const char *fmt)
{
	hxmc_t *str;
	int ret;

	if ((ret = HXformat2_aprintf(ftable, &str, fmt)) < 0)
		return ret;
	if (ret == 0) {
		*dest = '\0';
		return 0;
	}
	strncpy(dest, str, size);
	HXmc_free(str);
	return strlen(dest);
}
