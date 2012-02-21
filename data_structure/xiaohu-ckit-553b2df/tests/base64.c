/* CKit: Data structure and arithmetic implement
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
#include "test.h"

int
test_base64 (int argc, char *argv[])
{
  const char *s;
  char *enc;
  uchar_t *dec;

  if (argc > 1)
    {
      char buff[4096];
      FILE *fp = fopen (argv[1], "r");
      size_t n = fread (buff, 1, sizeof (buff), fp);
      enc = ckit_base64_encode ((uchar_t*) buff, n,
				CKIT_BASE64_CHARSET_DEFAULT);
      printf ("%s\n", enc);
      free (enc);
      fclose (fp);
    }
  s = "0000\x0a";
  enc = ckit_base64_encode ((uchar_t*) s, -1, CKIT_BASE64_CHARSET_DEFAULT);
  assert (strcmp ("MDAwMAo=", enc) == 0);
  dec = ckit_base64_decode (enc, -1, CKIT_BASE64_CHARSET_DEFAULT);
  assert (strcmp (s, dec) == 0);
  free (enc);
  free (dec);

  s = "0\x0a";
  enc = ckit_base64_encode ((uchar_t*) s, -1, CKIT_BASE64_CHARSET_DEFAULT);
  assert (strcmp ("MAo=", enc) == 0);
  free (enc);

  s = "11111\x0a";
  enc = ckit_base64_encode ((uchar_t*) s, -1, CKIT_BASE64_CHARSET_DEFAULT);
  assert (strcmp ("MTExMTEK", enc) == 0);
  free (enc);

  return 0;
}
