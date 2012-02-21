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

#ifndef CKIT_LOG_H
#define CKIT_LOG_H

#include <ckit/ckit-types.h>

CKIT_BEGIN_DECLS

/* log level mask */
#define CKIT_LOG_LEVEL_DBUG  0x01
#define CKIT_LOG_LEVEL_INFO  0x02
#define CKIT_LOG_LEVEL_WARN  0x04
#define CKIT_LOG_LEVEL_CRIT  0x08
#define CKIT_LOG_LEVEL_FATL  0x10

void  ckit_log_set_handler (ckit_log_handler_f handler);
void  ckit_log_set_level (uint32_t level);
void  ckit_log (uint32_t level, const char *format, ...);
void  ckit_logv (uint32_t level, const char *format, va_list args);
void  ckit_log_handler_syslog (uint32_t level, const char *message);

/* convenient macros to log functions */
#ifdef CKIT_HAVE_ISO_C_VARARGS
#define ckit_dbug(...)  ckit_log(CKIT_LOG_LEVEL_DBUG, __VA_ARGS__)
#define ckit_info(...)  ckit_log(CKIT_LOG_LEVEL_INFO, __VA_ARGS__)
#define ckit_warn(...)  ckit_log(CKIT_LOG_LEVEL_WARN, __VA_ARGS__)
#define ckit_crit(...)  ckit_log(CKIT_LOG_LEVEL_CRIT, __VA_ARGS__)
#define ckit_fatl(...)  ckit_log(CKIT_LOG_LEVEL_FATL, __VA_ARGS__)
#elif defined(CKIT_HAVE_GNUC_VARARGS)
#define ckit_dbug(format...)  ckit_log(CKIT_LOG_LEVEL_DBUG, format)
#define ckit_info(format...)  ckit_log(CKIT_LOG_LEVEL_INFO, format)
#define ckit_warn(format...)  ckit_log(CKIT_LOG_LEVEL_WARN, format)
#define ckit_crit(format...)  ckit_log(CKIT_LOG_LEVEL_CRIT, format)
#define ckit_fatl(format...)  ckit_log(CKIT_LOG_LEVEL_FATL, format)
#else
static void ckit_dbug (const char *format, ...) {
  va_list args;
  va_start (args, format);
  ckit_logv (CKIT_LOG_LEVEL_DBUG, format, args);
  va_end (args);
}
static void ckit_info (const char *format, ...) {
  va_list args;
  va_start (args, format);
  ckit_logv (CKIT_LOG_LEVEL_INFO, format, args);
  va_end (args);
}
static void ckit_warn (const char *format, ...) {
  va_list args;
  va_start (args, format);
  ckit_logv (CKIT_LOG_LEVEL_WARN, format, args);
  va_end (args);
}
static void ckit_crit (const char *format, ...) {
  va_list args;
  va_start (args, format);
  ckit_logv (CKIT_LOG_LEVEL_CRIT, format, args);
  va_end (args);
}
static void ckit_fatl (const char *format, ...) {
  va_list args;
  va_start (args, format);
  ckit_logv (CKIT_LOG_LEVEL_FATL, format, args);
  va_end (args);
}
#endif /* CKIT_HAVE_ISO_C_VARARGS */


# define ckit_return_if_fail(exp) do {				\
    if (exp) { } else {						\
      ckit_log (CKIT_LOG_LEVEL_WARN,				\
		"(%s:%d): %s: check failed: (%s)",		\
		__FILE__, __LINE__, CKIT_STRFUNC, #exp);	\
      return;							\
    }								\
  } while (0)
# define ckit_return_val_if_fail(exp,val) do {			\
    if (exp) {} else {						\
      ckit_log (CKIT_LOG_LEVEL_WARN,				\
		"(%s:%d): %s: check failed: (%s)",		\
		__FILE__, __LINE__, CKIT_STRFUNC, #exp);	\
      return (val);						\
    }								\
  } while (0)

CKIT_END_DECLS

#endif /* CKIT_LOG_H */
