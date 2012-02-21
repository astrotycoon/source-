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
test_version (int argc, char *argv[])
{
  assert (CKIT_CHECK_VERSION (CKIT_MAJOR_VERSION,
			      CKIT_MINOR_VERSION,
			      CKIT_MICRO_VERSION));
  assert (!CKIT_CHECK_VERSION (CKIT_MAJOR_VERSION + 1,
			       CKIT_MINOR_VERSION,
			       CKIT_MICRO_VERSION));
  assert (!CKIT_CHECK_VERSION (CKIT_MAJOR_VERSION,
			       CKIT_MINOR_VERSION + 1,
			       CKIT_MICRO_VERSION));
  assert (!CKIT_CHECK_VERSION (CKIT_MAJOR_VERSION,
			       CKIT_MINOR_VERSION,
			       CKIT_MICRO_VERSION + 1));

  assert (!ckit_check_version (CKIT_MAJOR_VERSION,
			       CKIT_MINOR_VERSION,
			       CKIT_MICRO_VERSION));
  assert (ckit_check_version (CKIT_MAJOR_VERSION + 1,
			      CKIT_MINOR_VERSION,
			      CKIT_MICRO_VERSION));
  assert (ckit_check_version (CKIT_MAJOR_VERSION,
			      CKIT_MINOR_VERSION + 1,
			      CKIT_MICRO_VERSION));
  assert (ckit_check_version (CKIT_MAJOR_VERSION,
			      CKIT_MINOR_VERSION,
			      CKIT_MICRO_VERSION + 1));

  return 0;
}
