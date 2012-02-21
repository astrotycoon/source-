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
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ckit/ckit.h"
#include <check.h>
#ifdef HAVE_SYSLOG_H
# include <syslog.h>
#endif

START_TEST (types_test)
{
  fail_if (false);
  fail_unless (true);
  printf ("sizeof(ckit_list_t): %u\n", (uint_t) sizeof (ckit_list_t));
  fail_unless (sizeof (ckit_list_t) == sizeof (void*) * 3);
  printf ("sizeof(ckit_node_t): %u\n", (uint_t) sizeof (ckit_node_t));
  fail_unless (sizeof (ckit_node_t) == sizeof (void*) * 5);
}
END_TEST


START_TEST (macros_test)
{
  ckit_list_t arr[3];
  fail_if (CKIT_N_ELEMENTS (arr) != 3);

  printf ("STRINGIFY(line): %s\n", CKIT_STRINGIFY(__LINE__));
  printf ("STRINGIFY(file): %s\n", CKIT_STRINGIFY(__FILE__));
  printf ("STRLOC: %s\n", CKIT_STRLOC);
  printf ("STRFUNC: %s\n", CKIT_STRFUNC);

  fail_unless (MAX (3, 5) == 5);
  fail_unless (MIN (3, 5) == 3);
  fail_unless (ABS (3) == 3);
  fail_unless (ABS (-3) == 3);
  fail_unless (CLAMP (100, -3, 3) == 3);
  fail_unless (CLAMP (-100, -3, 3) == -3);
}
END_TEST


static int
failed_with_val (int val)
{
  ckit_return_val_if_fail (val > 10, 0);
  return 1;
}

START_TEST (log_test)
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

  fail_if (failed_with_val (30) == 0);
  fail_unless (failed_with_val (3) == 0);
}
END_TEST


START_TEST (version_test)
{
  fail_unless (CKIT_CHECK_VERSION (CKIT_MAJOR_VERSION, CKIT_MINOR_VERSION,
				   CKIT_MICRO_VERSION));
  fail_if (CKIT_CHECK_VERSION (CKIT_MAJOR_VERSION + 1,
			       CKIT_MINOR_VERSION, CKIT_MICRO_VERSION));
  fail_if (CKIT_CHECK_VERSION (CKIT_MAJOR_VERSION, CKIT_MINOR_VERSION + 1,
			       CKIT_MICRO_VERSION));
  fail_if (CKIT_CHECK_VERSION (CKIT_MAJOR_VERSION, CKIT_MINOR_VERSION,
			       CKIT_MICRO_VERSION + 1));

  fail_if (ckit_check_version (CKIT_MAJOR_VERSION, CKIT_MINOR_VERSION,
			       CKIT_MICRO_VERSION) != NULL);
  fail_if (ckit_check_version (CKIT_MAJOR_VERSION + 1, CKIT_MINOR_VERSION,
			       CKIT_MICRO_VERSION) == NULL);
  fail_if (ckit_check_version (CKIT_MAJOR_VERSION, CKIT_MINOR_VERSION + 1,
			       CKIT_MICRO_VERSION) == NULL);
  fail_if (ckit_check_version (CKIT_MAJOR_VERSION, CKIT_MINOR_VERSION,
			       CKIT_MICRO_VERSION + 1) == NULL);
}
END_TEST


static Suite*
base_test_suite (void)
{
  Suite *suite = suite_create ("Base Test Suite");

  TCase *case_types = tcase_create ("Types");
  tcase_add_test (case_types, types_test);
  tcase_add_test (case_types, macros_test);
  tcase_add_test (case_types, log_test);
  tcase_add_test (case_types, version_test);
  suite_add_tcase (suite, case_types);

  return suite;
}


int
main (int argc, char *argv[])
{
  SRunner *runner = srunner_create (base_test_suite ());
  srunner_run_all (runner, CK_NORMAL);

  return (srunner_ntests_failed (runner) == 0) ?
   EXIT_SUCCESS : EXIT_FAILURE;
}
