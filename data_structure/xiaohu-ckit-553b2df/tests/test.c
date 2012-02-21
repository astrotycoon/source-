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

extern int test_types (int argc, char *argv[]);
extern int test_macros (int argc, char *argv[]);
extern int test_version (int argc, char *argv[]);
extern int test_log (int argc, char *argv[]);
extern int test_base16 (int argc, char *argv[]);
extern int test_base64 (int argc, char *argv[]);
extern int test_list (int argc, char *argv[]);
extern int test_sort (int argc, char *argv[]);

#define testcase(func) do { \
  printf ("--------- " #func " ----------\n"); \
  test_##func (argc, argv); \
} while (0);

int main (int argc, char *argv[])
{
  testcase (types);
  testcase (macros);
  testcase (version);
  testcase (log);
  testcase (base16);
  testcase (base64);
  testcase (list);
  testcase (sort);

  return 0;
}
