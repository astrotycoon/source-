/**
 * sorting algorithms test case driver
 * Copyright (c) 2010, Gaurav Mathur <narainmg@gmail.com>
 *   
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *   
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See README and COPYING for more details.
 */

#include <stdio.h>
#include <sort.h>

/**
* @brief insertion sort driver
 *
 * This routine implements a test driver for insertion sort
 *
 */
int isort_main (int argc, char** argv)
{
#define TEST_ISORT_BOUND   (1024*20)
   int arr[TEST_ISORT_BOUND];
   int idx;
   int val = 1;

   for (idx = TEST_ISORT_BOUND; idx >= 0; idx--)
   {
      arr[idx] = val;
      val++;
   }
   
   isort_i (arr, TEST_ISORT_BOUND);

   for (idx = 0; idx < TEST_ISORT_BOUND; idx++)
   {
      fprintf (stdout, "%d\n", arr[idx]);
   }
   return 0;
}

/**
 * @brief heapsort driver
 *
 * This routine implements a test driver for heapsort
 *
 */
int heapsort_main (int argc, char** argv)
{
#define TEST_HSORT_BOUND   (1024*20)
   int idx;
   HEAP_T* h;
   void* p;
   unsigned long key;
   unsigned long ctx = 0;
   
   h = heap_create (DS_HEAP_MAX, TEST_HSORT_BOUND);
   for (idx = TEST_HSORT_BOUND; idx > 0; idx--)
   {
      heap_max_insert (h, idx, NULL, NULL);
   }

   heapsort (h);
   heap_dump (h);

   while (HEAP_ERR_ITERATOR_DONE != heap_iter (h, &p, &key, &ctx))
   {
      fprintf (stdout, "key = %lu\n", key);
   }
   
   return 0;
}

