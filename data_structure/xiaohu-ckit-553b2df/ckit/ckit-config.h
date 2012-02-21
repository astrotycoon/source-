/* CKit: Data structure and arithmetic implement
 * System configure information
 * CKit release under the MIT License.
 * This file was generated by configure, DON'T modify.
 */
#ifndef CKIT_CONFIG_H
#define CKIT_CONFIG_H

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
#include <inttypes.h>
#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#define CKIT_HAVE_THREAD  1

/* Version number defines. */
#define CKIT_MAJOR_VERSION  0
#define CKIT_MINOR_VERSION  0
#define CKIT_MICRO_VERSION  1

#ifndef __cplusplus
# define CKIT_HAVE_ISO_VARARGS 1
#endif
#ifdef __cplusplus
# define CKIT_HAVE_ISO_VARARGS 1
#endif

/* gcc-2.95.x supports both gnu style and ISO varargs, but if -ansi
 * is passed ISO vararg support is turned off, and there is no work
 * around to turn it on, so we unconditionally turn it off. */
#if __GNUC__ == 2 && __GNUC_MINOR__ == 95
# undef CKIT_HAVE_ISO_VARARGS
#endif
#define CKIT_HAVE_GNUC_VARARGS 1

#include <stdbool.h>
#define bool_t  _Bool

#ifdef __cplusplus
}
#endif

#endif /* CKIT_CONFIG_H */
