#ifndef _LIBHX_MISC_H
#define _LIBHX_MISC_H 1

#ifndef __cplusplus
#	include <limits.h>
#	include <stdarg.h>
#	include <stdio.h>
#else
#	include <climits>
#	include <cstdarg>
#	include <cstdio>
#endif
#include <libHX/io.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	BITMAP.H
 */
#define __HXbitmap_bpq(type) \
	(sizeof(type) * CHAR_BIT)
#define HXbitmap_size(type, bits) \
	(((bits) + __HXbitmap_bpq(type) - 1) / __HXbitmap_bpq(type))
#define __HXbitmap_quant(map, bit) \
	((map)[(bit) / __HXbitmap_bpq(*(map))])
#define HXbitmap_set(map, bit) \
	((void)(__HXbitmap_quant((map), (bit)) |= (1ULL << ((bit) % __HXbitmap_bpq(*(map))))))
#define HXbitmap_clear(map, bit) \
	((void)(__HXbitmap_quant((map), (bit)) &= ~(1ULL << ((bit) % __HXbitmap_bpq(*(map))))))
#define HXbitmap_test(map, bit) \
	((bool)(__HXbitmap_quant((map), (bit)) & (1ULL << ((bit) % __HXbitmap_bpq(*(map))))))

struct stat;
struct timespec;
struct timeval;

/*
 *	DL.C
 */
extern void *HX_dlopen(const char *);
extern void *HX_dlsym(void *, const char *);
extern void HX_dlclose(void *);
extern const char *HX_dlerror(void);

/*
 *	MISC.C
 */
extern int HX_ffs(unsigned long);
extern int HX_fls(unsigned long);
extern void HX_hexdump(FILE *, const void *, unsigned int);
extern void HX_diff_timespec(struct timespec *,
	const struct timespec *, const struct timespec *);
extern void HX_diff_timeval(struct timeval *,
	const struct timeval *, const struct timeval *);
extern long HX_time_compare(const struct stat *, const struct stat *, char);
extern void HX_zvecfree(char **);

/*
 *	RAND.C
 */
extern int HX_rand(void);
extern unsigned int HX_irand(unsigned int, unsigned int);
extern double HX_drand(double, double);

/*
 *	INLINE FUNCTIONS
 */
static inline unsigned int HX_zveclen(const char *const *args)
{
	unsigned int argk = 0;
	while (*args++ != NULL)
		++argk;
	return argk;
}

#ifdef __cplusplus
} /* extern "C" */

extern "C++" {

template<typename type> static inline type
HX_dlsym(void *handle, const char *symbol)
{
	return reinterpret_cast<type>(HX_dlsym(handle, symbol));
}

} /* extern "C++" */
#endif

#endif /* _LIBHX_MISC_H */
