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

int test_base16 (int argc, char *argv[])
{
  char *base16;
  uchar_t *s = (uchar_t*) "abcdef", *str;

  if (argc > 1)
    {
      char buff[2048];
      FILE *fp = fopen (argv[1], "r");
      size_t size = fread (buff, 1, sizeof (buff), fp);
      if (!ferror (fp))
	{
	  base16 = ckit_base16_encode ((uchar_t*) buff, (int) size);
	  printf ("%zu vs %zu\n%s\n", size, strlen (base16), base16);
	  free (base16);
	}
      fclose (fp);
    }
  base16 = ckit_base16_encode (s, -1);
  printf ("base16_encode(%s)=%s\n", s, base16);
  str = ckit_base16_decode (base16, -1);
  assert (memcmp (str, s, 6) == 0);
  free (base16);
  free (str);

  s = (uchar_t*) "\xFF\xFF\xFE\xEF";
  base16 = ckit_base16_encode (s, -1);
  assert (strcmp (base16, "FFFFFEEF") == 0);
  str = ckit_base16_decode (base16, -1);
  assert (memcmp (str, s, 4) == 0);
  free (base16);
  free (str);

  return 0;
}
