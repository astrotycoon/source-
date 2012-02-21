/*
* Copyright (c) 2008, Björn Rehm (bjoern@shugaa.de)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

/** @file dll_util.h
 *
 * @brief Library utility functions
 *
 * */

#ifndef _DLL_UTIL_H
#define _DLL_UTIL_H

/* ######################################################################### */
/*                            TODO / Notes                                   */
/* ######################################################################### */

/* ######################################################################### */
/*                            Types & Defines                                */
/* ######################################################################### */

/* ######################################################################### */
/*                            Public interface                               */
/* ######################################################################### */

/** Get a descriptive text for a library error code
 *
 * @param errcode    Error code
 *
 * @return           Pointer to description string
 */
char const* dll_strerror(int errnum);

/** Simple comparator for signed integers
 * 
 * The implementation is trivial, it should give you a pretty good idea of how 
 * to write your own comparator functions for arbitrary data types.
 *
 * @param item1      Pointer to first item
 * @param item2      Pointer to second item
 *
 * @return 0         item1 == item2
 * @return 1         item1 > item2
 * @return -1        item1 < item2
 */
int dll_compar_int(const void *item1, const void *item2);

#endif /* _DLL_UTIL_H */
