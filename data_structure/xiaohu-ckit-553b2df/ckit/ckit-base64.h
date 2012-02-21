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

#ifndef CKIT_BASE64_H
#define CKIT_BASE64_H

#include <ckit/ckit-types.h>

CKIT_BEGIN_DECLS

#define _CKIT_BASE64_CHARSET_BASE      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcde" \
                                       "fghijklmnopqrstuvwxyz0123456789"

#define CKIT_BASE64_CHARSET_DEFAULT   _CKIT_BASE64_CHARSET_BASE "+/="
#define CKIT_BASE64_CHARSET_FILENAME  _CKIT_BASE64_CHARSET_BASE "-_="


char* ckit_base64_encode    (const uchar_t *data, size_t length,
			     const char *charsets) CKIT_WARN_UNUSED_RESULT;
uchar_t* ckit_base64_decode (const char *base64_data, size_t length,
			     const char *charsets,
			     size_t *result_size) CKIT_WARN_UNUSED_RESULT;

char* ckit_base32_encode    (const uchar_t *data,
			     size_t length) CKIT_WARN_UNUSED_RESULT;
uchar_t* ckit_base32_decode (const char *base32_data, size_t length,
			     size_t *result_size) CKIT_WARN_UNUSED_RESULT;

char* ckit_base16_encode    (const uchar_t *data,
			     size_t length) CKIT_WARN_UNUSED_RESULT;
uchar_t* ckit_base16_decode (const char *base16_data, size_t length,
			     size_t *result_size) CKIT_WARN_UNUSED_RESULT;

CKIT_END_DECLS

#endif /* CKIT_BASE64_H */
