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

#ifndef CKIT_MACROS_H
#define CKIT_MACROS_H

#include <ckit/ckit-config.h>

#ifndef CKIT_BEGIN_DECLS
#ifdef __cplusplus
# define CKIT_BEGIN_DECLS extern "C" {
# define CKIT_END_DECLS   }
#else
# define CKIT_BEGIN_DECLS
# define CKIT_END_DECLS
#endif
#endif /* CKIT_BEGIN_DECLS */

/* Some system maybe has not defined NULL. */
#ifndef NULL
# ifdef __cplusplus
#  define NULL        (0L)
# else
#  define NULL        ((void*) 0)
# endif
#endif

#define CKIT_N_ELEMENTS(array)  (sizeof (array) / sizeof (array[0]))

#define CKIT_STRINGIFY(macro_or_string)	CKIT_STRINGIFY_ARG(macro_or_string)
#define	CKIT_STRINGIFY_ARG(contents)	#contents

/* Provide a string identifying the current code position */
#if defined(__GNUC__) && (__GNUC__ < 3) && !defined(__cplusplus)
# define CKIT_STRLOC  __FILE__ ":" CKIT_STRINGIFY(__LINE__) ":" __PRETTY_FUNCTION__ "()"
#else
# define CKIT_STRLOC  __FILE__ ":" CKIT_STRINGIFY(__LINE__)
#endif

/* Provide a string identifying the current function, non-concatenatable */
#if defined (__GNUC__)
# define CKIT_STRFUNC     ((const char*)(__PRETTY_FUNCTION__))
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 19901L
# define CKIT_STRFUNC     ((const char*) (__func__))
#else
# define CKIT_STRFUNC     ((const char*) ("???"))
#endif

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#define CKIT_WARN_UNUSED_RESULT  __attribute__((warn_unused_result))
#else
#define CKIT_WARN_UNUSED_RESULT
#endif

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
# define CKIT_PRINTF(format_idx, arg_idx) \
 __attribute__((__format__ (__printf__, format_idx, arg_idx)))
# define CKIT_SCANF(format_idx, arg_idx) \
 __attribute__((__format__ (__scanf__, format_idx, arg_idx)))
# define CKIT_FORMAT(arg_idx)  __attribute__((__format_arg__ (arg_idx)))
# define CKIT_NORETURN  __attribute__((__noreturn__))
# define CKIT_CONST  __attribute__((__const__))
# define CKIT_UNUSED  __attribute__((__unused__))
# define CKIT_NO_INSTRUMENT  __attribute__((__no_instrument_function__))
#else
# define CKIT_PRINTF(format_idx, arg_idx)
# define CKIT_SCANF(format_idx, arg_idx)
# define CKIT_FORMAT(arg_idx)
# define CKIT_NORETURN
# define CKIT_CONST
# define CKIT_UNUSED
# define CKIT_NO_INSTRUMENT
#endif

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
# define CKIT_DEPRECATED  __attribute__((__deprecated__))
#else
# define CKIT_DEPRECATED
#endif /* __GNUC__ */

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
# define CKIT_DEPRECATED_FOR(f) \
 __attribute__((deprecated("Use " #f " instead")))
#else
# define CKIT_DEPRECATED_FOR(f)        CKIT_DEPRECATED
#endif /* __GNUC__ */

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
# define CKIT_MAY_ALIAS  __attribute__((may_alias))
#else
# define CKIT_MAY_ALIAS
#endif /* __GNUC__ */

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
# define CKIT_PURE   __attribute__((__pure__))
# define CKIT_MALLOC __attribute__((__malloc__))
#else
# define CKIT_PURE
# define CKIT_MALLOC
#endif /* __GNUC__ */

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)
# define CKIT_ALLOC_SIZE(x)    __attribute__((__alloc_size__(x)))
# define CKIT_ALLOC_SIZE2(x,y) __attribute__((__alloc_size__(x,y)))
#else
# define CKIT_ALLOC_SIZE(x)
# define CKIT_ALLOC_SIZE2(x,y)
#endif /* __GNUC__ */

#undef	MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#undef	MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#undef	ABS
#define ABS(a)	   (((a) < 0) ? -(a) : (a))

#undef	CLAMP
#define CLAMP(x, low, high)  \
 (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#define CKIT_PTOI(p)  ((int) (intptr_t) (p))
#define CKIT_PTOU(p)  ((uint_t) (uintptr_t) (p))
#define CKIT_ITOP(i)  ((void*) (intptr_t) (i))
#define CKIT_UTOP(u)  ((void*) (uintptr_t) (u))

/* Provide convenience macros for handling structure
 * fields through their offsets. */
#if defined(__GNUC__)  && __GNUC__ >= 4
# define CKIT_STRUCT_OFFSET(struct_type, member) \
      ((long) offsetof (struct_type, member))
#else
# define CKIT_STRUCT_OFFSET(struct_type, member)	\
      ((long) ((unsigned char*) &((struct_type*) 0)->member))
#endif

#define CKIT_STRUCT_MEMBER_P(struct_p, struct_offset)   \
    ((void*) ((unsigned char*) (struct_p) + (long) (struct_offset)))
#define CKIT_STRUCT_MEMBER(member_type, struct_p, struct_offset)   \
    (*(member_type*) _STRUCT_MEMBER_P_ ((struct_p), (struct_offset)))

#endif /* CKIT_MACROS_H */
