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

static int
failed_with_val (int val)
{
  ckit_return_val_if_fail (val > 10, 0);
  return 1;
}

int test_log (int argc, char *argv[])
{
  ckit_log_set_level (0xFF);

  ckit_log (CKIT_LOG_LEVEL_DBUG, "it is a debug message");
  ckit_log (CKIT_LOG_LEVEL_INFO, "it is a info message");
  ckit_log (CKIT_LOG_LEVEL_WARN, "it is a warning message");
  ckit_log (CKIT_LOG_LEVEL_CRIT, "it is a critical message");

  openlog ("ckit-log-test", LOG_PERROR | LOG_PID, LOG_USER);
  ckit_log_set_handler (ckit_log_handler_syslog);

  ckit_log (CKIT_LOG_LEVEL_DBUG, "it is a debug message");
  ckit_log (CKIT_LOG_LEVEL_INFO, "it is a info message");
  ckit_log (CKIT_LOG_LEVEL_WARN, "it is a warning message");
  ckit_log (CKIT_LOG_LEVEL_CRIT, "it is a critical message");
  //ckit_log (CKIT_LOG_LEVEL_FATL, "it is a fatal message");

  closelog ();
  ckit_log_set_handler (NULL);

  ckit_dbug ("it is a debug message");
  ckit_info ("it is a info message");
  ckit_warn ("it is a warning message");
  ckit_crit ("it is a critical message");
  //ckit_fatl ("it is a fatal message");

  failed_with_val (30);
  failed_with_val (3);

  return 0;
}
