/* CKit: Data structure and arithmetic implement in C
 * Copyright (C) 2011 Wu Xiaohu, All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#if !defined (__CKIT_H_INSIDE__) && !defined (CKIT_COMPILATION)
# error "Only <ckit/ckit.h> can be included directly."
#endif

#ifndef CKIT_TYPES_H
#define CKIT_TYPES_H

#include <ckit/ckit-macros.h>

CKIT_BEGIN_DECLS

typedef unsigned char   uchar_t;
typedef unsigned short  ushort_t;
typedef unsigned int    uint_t;
typedef unsigned long   ulong_t;

/* callback function prototype */
typedef bool_t (*ckit_foreach_f) (void *data, void *userdata);
typedef int (*ckit_compare_f) (const void *data1, const void *data2);
typedef int (*ckit_compare2_f) (void *userdata, const void *data1, const void *data2);
typedef void (*ckit_free_f) (void *data);
typedef void (*ckit_copy_f) (const void *data, void *userdata);
typedef void (*ckit_log_handler_f) (uint32_t level, const char *message);
typedef intmax_t (*ckit_hash_f) (const void *data);

#ifdef CKIT_PLATFORM_WIN32
# ifndef CKIT_STATIC_COMPILATION
#  ifdef CKIT_COMPILATION
#   define CKIT_EXTERN extern __declspec(dllexport)
#  else
#   define CKIT_EXTERN extern __declspec(dllimport)
#  endif
# endif /* CKIT_STATIC_COMPILATION */
#endif /* CKIT_PLATFORM_WIN32 */

#ifndef CKIT_EXTERN
# define CKIT_EXTERN  extern
#endif

CKIT_END_DECLS

/* for dmalloc memory analysis */
#if defined(WITH_DMALLOC) && defined(DEBUG)
# include <dmalloc.h>
#endif

#endif /* CKIT_TYPES_H */
