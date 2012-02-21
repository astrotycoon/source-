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

#include "ckit-log.h"
#include "ckit-base64.h"

/* RFC 3548: The Base16, Base32, Base64 Data Encodings */

char*
ckit_base64_encode (const uchar_t *data, size_t length,
		    const char *charset)
{
  char *result;
  size_t reslen, i, j;
  uint32_t word = 0;
  uchar_t *p = (uchar_t*) &word;
  bool_t pad1 = false, pad2 = false;

  ckit_return_val_if_fail (data, NULL);
  ckit_return_val_if_fail (charset, NULL);
  ckit_return_val_if_fail (strlen (charset) >= 65, NULL);

  length = (length >= 0) ? length : strlen ((char*) data);
  if (length == 0)
    return NULL;

  reslen = (length / 3) * 4 + ((length % 3) ? 5 : 1);
  result = malloc (reslen);
  result[reslen - 1] = '\0';

  for (i = 0, j = 0; i < length && j < reslen;)
    {
      p[2] = data[i++];
      p[1] = (i == length) ? (pad2 = true, 0) : data[i++];
      p[0] = (i == length) ? (pad1 = true, 0) : data[i++];

      result[j++] = charset[(word >> 18) & 0x3F];
      result[j++] = charset[(word >> 12) & 0x3F];
      result[j++] = pad2 ? charset[64] : charset[(word >> 6) & 0x3F];
      result[j++] = pad1 ? charset[64] : charset[word & 0x3F];
    }
  return result;
}


uchar_t*
ckit_base64_decode (const char *base64_data, size_t length,
		    const char *charset, size_t *result_size)
{
  uchar_t *result;
  size_t reslen, i, j;
  uint32_t word, idx;
  uchar_t *p = (uchar_t*) &word;

  ckit_return_val_if_fail (base64_data, NULL);
  ckit_return_val_if_fail (charset, NULL);
  ckit_return_val_if_fail (strlen (charset) >= 65, NULL);

  length = (length >= 0) ? length : strlen (base64_data);
  if (length == 0)
    return NULL;

  reslen = (length / 4) * 3 + ((length % 4) ? 4 : 1);
  result = malloc (reslen);
  result[reslen - 1] = '\0';
  if (result_size)
    *result_size = reslen - 1;

#define index_of_char_(_charset, _ch, _off, _idx) do {	       		\
    for (_idx = 0; _idx <= 64; _idx++)					\
      {									\
	if (_charset[_idx] == _ch)					\
	  break;							\
      }									\
    if (_idx > 64)							\
      {									\
	ckit_warn ("Unrecognizable Base64 character '%c' " 		\
		   "at offset %d", _ch, _off);				\
	free (result);							\
	return NULL;							\
      }									\
  } while (0)

  for (i = 0, j = 0; i < length && j < reslen;)
    {
      index_of_char_ (charset, base64_data[i], i, idx);
      word = idx, i++;
      index_of_char_ (charset, base64_data[i], i, idx);
      word = (word << 6) + idx, i++;
      index_of_char_ (charset, base64_data[i], i, idx);
      word = (idx == 64) ? (word << 6) : (word << 6) + idx, i++;
      if (idx == 64 && result_size) *result_size -= 1;
      index_of_char_ (charset, base64_data[i], i, idx);
      word = (idx == 64) ? (word << 6) : (word << 6) + idx, i++;
      if (idx == 64 && result_size) *result_size -= 1;

      result[j++] = p[2];
      result[j++] = p[1];
      result[j++] = p[0];
    }
  return result;
}


static const char *_base32_charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ012345=";

char*
ckit_base32_encode (const uchar_t *data, size_t length)
{
  char *result;
  size_t reslen, i, j;
  uint64_t word = 0;
  uchar_t *p = (uchar_t*) &word;
  bool_t pad1 = false, pad2 = false, pad3 = false, pad4 = false;

  ckit_return_val_if_fail (data, NULL);

  length = (length >= 0) ? length : strlen ((char*) data);
  if (length == 0)
    return NULL;

  reslen = (length / 5) * 8 + ((length % 5) ? 9 : 1);
  result = malloc (reslen);
  result[reslen - 1] = '\0';

  for (i = 0, j = 0; i < length && j < reslen;)
    {
      p[4] = data[i++];
      p[3] = (i == length) ? (pad4 = true, 0) : data[i++];
      p[2] = (i == length) ? (pad3 = true, 0) : data[i++];
      p[1] = (i == length) ? (pad2 = true, 0) : data[i++];
      p[0] = (i == length) ? (pad1 = true, 0) : data[i++];

      result[j++] = _base32_charset[(word >> 35) & 0x1F];
      result[j++] = _base32_charset[(word >> 30) & 0x1F];
      result[j++] = pad4 ? _base32_charset[32] :
                    _base32_charset[(word >> 25) & 0x1F];
      result[j++] = pad4 ? _base32_charset[32] :
                    _base32_charset[(word >> 20) & 0x1F];
      result[j++] = pad3 ? _base32_charset[32] :
                    _base32_charset[(word >> 15) & 0x1F];
      result[j++] = pad2 ? _base32_charset[32] :
                    _base32_charset[(word >> 10) & 0x1F];
      result[j++] = pad2 ? _base32_charset[32] :
                    _base32_charset[(word >> 5) & 0x1F];
      result[j++] = pad1 ? _base32_charset[32] :
                    _base32_charset[word & 0x1F];
    }
  return result;
}


uchar_t*
ckit_base32_decode (const char *base32_data, size_t length,
		    size_t *result_size)
{
  uchar_t *result;
  size_t reslen, i, j;
  uint64_t word, idx;
  uchar_t *p = (uchar_t*) &word;

  ckit_return_val_if_fail (base32_data, NULL);

  length = (length >= 0) ? length : strlen (base32_data);
  if (length == 0)
    return NULL;

  reslen = (length / 8) * 5 + ((length % 8) ? 6 : 1);
  result = malloc (reslen);
  result[reslen - 1] = '\0';
  if (result_size)
    *result_size = reslen - 1;

#define _base32_index_of(_ch, _idx) \
      _idx = (_ch >= 'A' && _ch <= 'Z') ?  _ch - 'A' : \
	     (_ch >= '0' && _ch <= '5') ?  _ch - '0' + 26 : 32;

  for (i = 0, j = 0; i < length && j < reslen;)
    {
      _base32_index_of (base32_data[i], idx);
      word = idx, i++;
      _base32_index_of (base32_data[i], idx);
      word = (word << 5) + idx, i++;
      _base32_index_of (base32_data[i], idx);
      word = (word == 32) ? (word << 5) : (word << 5) + idx, i++;
      if (idx == 32 && result_size) *result_size -= 1;
      _base32_index_of (base32_data[i], idx);
      word = (word == 32) ? (word << 5) : (word << 5) + idx, i++;
      if (idx == 32 && result_size) *result_size -= 1;
      _base32_index_of (base32_data[i], idx);
      word = (word == 32) ? (word << 5) : (word << 5) + idx, i++;
      if (idx == 32 && result_size) *result_size -= 1;
      _base32_index_of (base32_data[i], idx);
      word = (word == 32) ? (word << 5) : (word << 5) + idx, i++;
      if (idx == 32 && result_size) *result_size -= 1;
      _base32_index_of (base32_data[i], idx);
      word = (word == 32) ? (word << 5) : (word << 5) + idx, i++;
      if (idx == 32 && result_size) *result_size -= 1;
      _base32_index_of (base32_data[i], idx);
      word = (word == 32) ? (word << 5) : (word << 5) + idx, i++;
      if (idx == 32 && result_size) *result_size -= 1;

      result[j++] = p[4];
      result[j++] = p[3];
      result[j++] = p[2];
      result[j++] = p[1];
      result[j++] = p[0];
    }
  return result;
}


char*
ckit_base16_encode (const uchar_t *data, size_t length)
{
  char *result;
  size_t i;

  ckit_return_val_if_fail (data, NULL);

  length = (length < 0) ? strlen ((char*) data) : length;
  if (length == 0)
    return NULL;

  result = malloc (length * 2 + 1);
  result[length * 2] = '\0';

  for (i = 0; i < length; i++)
    {
      uchar_t c = (data[i] >> 4);
      result[i * 2] = (c > 9) ? c - 10 + 'A' : c + '0';
      c = (data[i] & 0x0F);
      result[i * 2 + 1] = (c > 9) ? c - 10 + 'A' : c + '0';
    }
  return result;
}


uchar_t*
ckit_base16_decode (const char *base16_data, size_t length,
		    size_t *result_size)
{
  uchar_t *result;
  size_t i;

  ckit_return_val_if_fail (base16_data, NULL);

  length = (length < 0) ? strlen (base16_data) : length;
  if (length == 0)
    return NULL;

  result = malloc (length / 2 + 1);
  result[length / 2] = '\0';
  if (result_size)
    *result_size = length / 2;

  for (i = 0; i < length / 2; i++)
    {
      char c = base16_data[i * 2];
      result[i] = ((c < 'A') ? c - '0' : c - 'A' + 10) << 4;
      c = base16_data[i * 2 + 1];
      result[i] += ((c < 'A') ? c - '0' : c - 'A' + 10);
    }
  return result;
}
