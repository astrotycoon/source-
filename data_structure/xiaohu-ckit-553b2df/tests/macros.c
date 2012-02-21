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

int test_macros (int argc, char *argv[])
{
  int arr[3];
  assert (CKIT_N_ELEMENTS (arr) == 3);

  printf ("STRINGIFY(line): %s\n", CKIT_STRINGIFY(__LINE__));
  printf ("STRINGIFY(file): %s\n", CKIT_STRINGIFY(__FILE__));
  printf ("STRLOC: %s\n", CKIT_STRLOC);
  printf ("STRFUNC: %s\n", CKIT_STRFUNC);

  assert (MAX (3, 5) == 5);
  assert (MIN (3, 5) == 3);
  assert (ABS (3) == 3);
  assert (ABS (-3) == 3);
  assert (CLAMP (100, -3, 3) == 3);
  assert (CLAMP (-100, -3, 3) == -3);

  return 0;
}
