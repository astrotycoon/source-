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
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#ifdef HAVE_SYSLOG_H
# include <syslog.h>
#endif

#include "ckit-log.h"

static void ckit_log_handler_default (uint32_t level, const char *message);

/* global variables. */
static ckit_log_handler_f  _log_handler = ckit_log_handler_default;
static uint32_t            _log_level   = 0xFF;


static inline void
ckit_log_level_format (int istty, uint32_t level,
		       char *string, size_t string_length)
{
  const char *fmt;

  if (istty)
    fmt = "\033[%dm%s\033[0m";
  else
    fmt = "%02d%s";

  if (level & CKIT_LOG_LEVEL_FATL)
    snprintf (string, string_length, fmt, 31, "FATL");
  else if (level & CKIT_LOG_LEVEL_CRIT)
    snprintf (string, string_length, fmt, 35, "CRIT");
  else if (level & CKIT_LOG_LEVEL_WARN)
    snprintf (string, string_length, fmt, 34, "WARN");
  else if (level & CKIT_LOG_LEVEL_INFO)
    snprintf (string, string_length, fmt, 0, "INFO");
  else if (level & CKIT_LOG_LEVEL_DBUG)
    snprintf (string, string_length, fmt, 33, "DBUG");
  else
    memset (string, 0, string_length);

  if (!istty)
    {
      char *p = strdup (string);
      strcpy (string, p + 2);
      free (p);
    }
}


static void
ckit_log_handler_default (uint32_t level, const char *message)
{
  struct timeval tv;
  struct tm *tmp;
  char timestr[32], levelstr[32];

  gettimeofday (&tv, NULL);
  tmp = localtime (&tv.tv_sec);
  strftime (timestr, sizeof (timestr), "%F %T", tmp);

  ckit_log_level_format (isatty (fileno (stdout)), level,
			 levelstr, sizeof (levelstr));

  fprintf (stdout, "[%d][%s.%lu][%s] %s\n", getpid (),
	   timestr, (ulong_t) tv.tv_usec, levelstr, message);
}


void
ckit_log_handler_syslog (uint32_t level, const char *message)
{
  int priority = LOG_INFO;

  if (level & CKIT_LOG_LEVEL_DBUG)
    priority = LOG_DEBUG;
  else if (level & CKIT_LOG_LEVEL_INFO)
    priority = LOG_INFO;
  else if (level & CKIT_LOG_LEVEL_WARN)
    priority = LOG_WARNING;
  else if (level & CKIT_LOG_LEVEL_CRIT)
    priority = LOG_CRIT;
  else if (level & CKIT_LOG_LEVEL_FATL)
    priority = LOG_EMERG;

  syslog (priority, message);
}


void
ckit_log_set_handler (ckit_log_handler_f handler)
{
  if (handler)
    _log_handler = handler;
  else
    _log_handler = ckit_log_handler_default;
}


void
ckit_log_set_level (uint32_t level)
{
  _log_level = level;
}


void
ckit_log (uint32_t level, const char *format, ...)
{
  va_list args;

  va_start (args, format);
  ckit_logv (level, format, args);
  va_end (args);
}


void
ckit_logv (uint32_t level, const char *format, va_list args)
{
  static int depth = 0;
  char message[1024];

#ifndef DEBUG
  /* ignore debug message if not in debug mode */
  if (level == CKIT_LOG_LEVEL_DBUG)
    return;
#endif

  /* filter message outside level but fatal message */
  if (!(level & CKIT_LOG_LEVEL_FATL) && !(level & _log_level))
    return;

  if (++depth >= 2)
    {
      fprintf (stdout, "ckit_log() recursive call...\n");
      depth = 0;
      return;
    }
  vsnprintf (message, sizeof (message), format, args);
  _log_handler (level, message);

  depth--;

  /* abort the process when receive fatal message */
  if (level & CKIT_LOG_LEVEL_FATL)
    abort ();
}
