#ifndef _LIBHX_IO_H
#define _LIBHX_IO_H 1

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __libhx_internal_hxmc_t_defined
#define __libhx_internal_hxmc_t_defined 1
typedef char hxmc_t;
#endif

enum {
	HXF_UID  = 1 << 0,
	HXF_GID  = 1 << 1,
	HXF_KEEP = 1 << 2,
	HX_REALPATH_NOFLAGS  = 0,
	HX_REALPATH_ABSOLUTE = 1 << 0,
	HX_REALPATH_SELF     = 1 << 1,
	HX_REALPATH_PARENT   = 1 << 2,
	HX_REALPATH_SYMLINK  = 1 << 3,
	HX_REALPATH_DEFAULT  =
		HX_REALPATH_SELF | HX_REALPATH_PARENT | HX_REALPATH_SYMLINK,
};

struct HXdir;

extern struct HXdir *HXdir_open(const char *);
extern const char *HXdir_read(struct HXdir *);
extern void HXdir_close(struct HXdir *);
extern int HX_copy_dir(const char *, const char *, unsigned int, ...);
extern int HX_copy_file(const char *, const char *, unsigned int, ...);
extern int HX_mkdir(const char *);
extern int HX_readlink(hxmc_t **, const char *);
extern int HX_realpath(hxmc_t **, const char *, unsigned int);
extern int HX_rrmdir(const char *);

extern ssize_t HXio_fullread(int, void *, size_t);
extern ssize_t HXio_fullwrite(int, const void *, size_t);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBHX_IO_H */
