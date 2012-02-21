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

#include "ckit-version.h"

/* *_VERSION comes from config.h */
const int ckit_major_version = MAJOR_VERSION;
const int ckit_minor_version = MINOR_VERSION;
const int ckit_micro_version = MICRO_VERSION;

const char*
ckit_check_version (int major_required,
		    int minor_required,
		    int micro_required)
{
  if (major_required > MAJOR_VERSION)
    return "CKit version too old (major mismatch)";
  if (major_required < MAJOR_VERSION)
    return "CKit version too new (major mismatch)";
  if (minor_required > MINOR_VERSION)
    return "CKit version too old (minor mismatch)";
  if (micro_required > MICRO_VERSION)
    return "CKit version too old (micro mismatch)";

  return NULL;
}
