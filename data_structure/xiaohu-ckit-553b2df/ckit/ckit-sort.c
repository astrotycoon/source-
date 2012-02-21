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

#include "ckit-sort.h"

void
ckit_qsort (void *base, size_t nel, size_t width, ckit_compare_f func)
{
#ifdef HAVE_QSORT
  qsort (base, nel, width, func);
#else
# warning "ckit_qsort unimplement."
#endif
}


void
ckit_qsort_r (void *base, size_t nel, size_t width, void *userdata,
	      ckit_compare2_f func)
{
#ifdef HAVE_QSORT_R
  qsort_r (base, nel, width, userdata, func);
#else
# warning "ckit_qsort_r unimplement."
#endif
}


int
ckit_heapsort (void *base, size_t nel, size_t width, ckit_compare_f func)
{
#ifdef HAVE_HEAPSORT
  return heapsort (base, nel, width, func);
#else
# warning "ckit_heapsort unimplement."
#endif
}


int
ckit_mergesort (void *base, size_t nel, size_t width, ckit_compare_f func)
{
#ifdef HAVE_HEAPSORT
  return mergesort (base, nel, width, func);
#else
# warning "ckit_mergesort unimplement."
#endif
}
