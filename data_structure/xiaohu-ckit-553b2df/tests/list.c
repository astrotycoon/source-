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

bool_t
print_int_list (void *data, void *userdata)
{
  printf ("%d, ", CKIT_PTOI (data));
  return true;
}


static inline void
test_insert_op (void)
{
  ckit_list_t *list = NULL;
  ckit_list_t *list2 = NULL;
  uint_t len;
  int i;

  list = ckit_list_append (list, CKIT_ITOP(3));
  assert (ckit_list_length (list) == 1);
  assert (CKIT_PTOI (ckit_list_nth_data (list, 0)) == 3);
  assert (CKIT_PTOI (ckit_list_nth_data (list, -1)) == 3);

  list = ckit_list_prepend (list, CKIT_ITOP(2));
  assert (ckit_list_length (list) == 2);
  assert (CKIT_PTOI (ckit_list_nth_data (list, 0)) == 2);
  assert (CKIT_PTOI (ckit_list_nth_data (list, -1)) == 3);

  list = ckit_list_insert (list, CKIT_ITOP(1), 0);
  assert (ckit_list_length (list) == 3);
  assert (CKIT_PTOI (ckit_list_nth_data (list, 0)) == 1);
  assert (CKIT_PTOI (ckit_list_nth_data (list, -1)) == 3);

  list = ckit_list_insert (list, CKIT_ITOP(4), -1);
  assert (ckit_list_length (list) == 4);
  assert (CKIT_PTOI (ckit_list_nth_data (list, 0)) == 1);
  assert (CKIT_PTOI (ckit_list_nth_data (list, -1)) == 4);

  list = ckit_list_insert (list, CKIT_ITOP(5), -1);
  list = ckit_list_insert (list, CKIT_ITOP(6), -1);
  list = ckit_list_insert (list, CKIT_ITOP(7), -1);

  list2 = ckit_list_copy (list);
  assert (ckit_list_length (list) == ckit_list_length (list2));
  list2 = ckit_list_reverse (list2);

#if 0
  printf ("list Contents: { ");
  ckit_list_foreach (list, print_int_list, NULL);
  printf ("}\n");

  printf ("reverse Contents: { ");
  ckit_list_foreach (list2, print_int_list, NULL);
  printf ("}\n");
#endif

  len = ckit_list_length (list);
  for (i = 0; i < len; i++)
    {
      assert (CKIT_PTOI(ckit_list_nth_data (list, i)) ==
	      CKIT_PTOI(ckit_list_nth_data (list2, len - i - 1)));
    }
  ckit_list_free (list2);
  ckit_list_free (list);
}


static inline void
test_remove_op (void)
{
  ckit_list_t *list = NULL;
  int i;

  for (i = 0; i < 10000; i++)
    list = ckit_list_append (list, CKIT_ITOP(i));
  assert (CKIT_PTOI (ckit_list_nth_data (list, 0)) == 0);
  assert (CKIT_PTOI (ckit_list_nth_data (list, -1)) == 10000 - 1);
  assert (ckit_list_length (list) == 10000);

  list = ckit_list_remove (list, CKIT_ITOP(1000001));
  assert (ckit_list_length (list) == 10000);

  for (i = 1; i < 9999; i++)
    list = ckit_list_remove (list, CKIT_ITOP(i));
  assert (ckit_list_length (list) == 2);
  assert (CKIT_PTOI (ckit_list_nth_data (list, 0)) == 0);
  assert (CKIT_PTOI (ckit_list_nth_data (list, -1)) == 10000 - 1);

  list = ckit_list_remove_index (list, -1, NULL);
  assert (ckit_list_length (list) == 1);
  assert (CKIT_PTOI (ckit_list_nth_data (list, 0)) == 0);

  list = ckit_list_remove_index (list, -1, NULL);
  assert (list == NULL);

  for (i = 0; i < 10000; i++)
    list = ckit_list_prepend (list, CKIT_ITOP(100));
  assert (ckit_list_length (list) == 10000);

  list = ckit_list_delete_node (list, list);
  assert (ckit_list_length (list) == 10000 - 1);

  list = ckit_list_delete_node (list, ckit_list_last (list));
  assert (ckit_list_length (list) == 10000 - 2);

  list = ckit_list_remove_all (list, CKIT_ITOP(100));
  assert (list == NULL);

  ckit_list_free (list);
}


static void
test_node_op (void)
{
  ckit_list_t *list = NULL, *node;

  list = ckit_list_append (list, CKIT_ITOP(100));
  list = ckit_list_append (list, CKIT_ITOP(101));

  node = ckit_list_next (list);
  assert (CKIT_PTOI(ckit_list_data (node)) == 101);

  node = ckit_list_prev (node);
  assert (CKIT_PTOI(ckit_list_data (node)) == 100);
  assert (node == list);

  ckit_list_free (list);
}


static int
find_node (const void *data, const void *userdata)
{
  return CKIT_PTOI (data) - CKIT_PTOI (userdata);
}

static void
test_find_op (void)
{
  ckit_list_t *list = NULL, *node;
  int i;

  for (i = 0; i < 10000; i++)
    list = ckit_list_append (list, CKIT_ITOP(i));

  node = ckit_list_find (list, CKIT_ITOP (100));
  assert (node);
  assert (CKIT_PTOI(ckit_list_data (node)) == 100);
  assert (ckit_list_position (list, node) == 100);

  node = ckit_list_find_custom (list, CKIT_ITOP (100), find_node);
  assert (node);
  assert (CKIT_PTOI(ckit_list_data (node)) == 100);
  assert (ckit_list_position (list, node) == 100);

  list = ckit_list_delete_node (list, node);
  assert (ckit_list_length (list) == 9999);

  ckit_list_free (list);
}

#ifdef HAVE_LIBPTHREAD
static void*
thread1 (void *data)
{
  ckit_list_t *list = (ckit_list_t*) data;
  int i;

  for (i = 50; i > 0; i--)
    list = ckit_list_prepend (list, CKIT_ITOP(i));

  for (i = 99; i > 50; i--)
    list = ckit_list_insert (list, CKIT_ITOP(i), 50);

  return NULL;
}

static void*
thread2 (void *data)
{
  ckit_list_t *list = (ckit_list_t*) data;
  int i;

  for (i = 101; i < 200; i++)
    list = ckit_list_append (list, CKIT_ITOP(i));

  return NULL;
}

static void
test_thread_op (void)
{
  pthread_t t1, t2;
  pthread_attr_t tattr;
  ckit_list_t *list = NULL, *node;

  list = ckit_list_prepend (list, CKIT_ITOP(100));

  pthread_attr_init (&tattr);
  pthread_attr_setdetachstate (&tattr, PTHREAD_CREATE_JOINABLE);
  assert (pthread_create (&t1, &tattr, thread1, list) == 0);
  assert (pthread_create (&t2, &tattr, thread2, list) == 0);
  pthread_join (t1, NULL);
  pthread_join (t2, NULL);

  /* we have to go back to the first node since thread has
   * prepend node to the list */
  list = ckit_list_first (list);
  for (node = list; node && node->next; node = node->next)
    assert (CKIT_PTOI (node->data) < CKIT_PTOI (node->next->data));

#if 0
  printf ("Contents after thread: { ");
  ckit_list_foreach (list, print_int_list, NULL);
  printf ("}\n");
#endif

  ckit_list_free (list);
}
#endif


int test_list (int argc, char *argv[])
{
  test_insert_op ();
  test_remove_op ();
  test_node_op ();
  test_find_op ();
#ifdef HAVE_LIBPTHREAD
  test_thread_op ();
#endif

  return 0;
}
