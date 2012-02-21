/**
 * heap test cases
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
#include <heap.h>

typedef struct {
   unsigned long num;
   unsigned long idx;
} TC_HEAP_DATA_SET;

void tc_heap_iterator (void)
{
   TC_HEAP_DATA_SET A[] = {{16, 0},
                            {4, 0},
                            {10, 0},
                            {14, 0},
                            {7, 0},
                            {9, 0},
                            {3, 0},
                            {2, 0},
                            {8, 0},
                            {1, 0}};
                            
   /* TC_HEAP_DATA_SET A[] = {{16, 0}}; */
   /* TC_HEAP_DATA_SET A[] = {}; */
   
   HEAP_T* h;
   unsigned int length;
   unsigned int idx;
   void* p;
   unsigned long key;
   unsigned long ctx = 0;
   
   h = heap_create (DS_HEAP_MAX, 14);
   length = sizeof (A)/sizeof (TC_HEAP_DATA_SET);
   for (idx = 0; idx < length; idx++)
   {
      if (HEAP_ERR_ERR == heap_max_insert (h, A[idx].num, NULL, &A[idx].idx))
         fprintf (stderr, "error adding heap element\n");
   }
   heap_graphviz_description (h, "heap-tc_heap_iterator.lst");

   while (HEAP_ERR_ITERATOR_DONE != heap_iter (h, &p, &key, &ctx))
   {
      fprintf (stdout, "key = %lu\n", key);
   }
}

/**
 * @brief test heap API
 *
 */
void tc_heap_max_main (void)
{
   /*
   TC_HEAP_DATA_SET A[] = {{12,0},
                           {11,0},
                           {10,0},
                           {9,0},
                           {6,0},
                           {7,0},
                           {8,0},
                           {5,0},
                           {1,0},
                           {4,0},
                           {14,0}};
   */                           
   TC_HEAP_DATA_SET A[] = {{16, 0},
                            {4, 0},
                            {10, 0},
                            {14, 0},
                            {7, 0},
                            {9, 0},
                            {3, 0},
                            {2, 0},
                            {8, 0},
                            {1, 0}};
  // int A[] = {4, 1, 3, 2, 16, 9, 10, 14, 8 ,7};
   HEAP_T* h;
   unsigned int length;
   unsigned int idx;
   
   h = heap_create (DS_HEAP_MAX, 14);
   length = sizeof (A)/sizeof (TC_HEAP_DATA_SET);
   for (idx = 0; idx < length; idx++)
   {
      if (HEAP_ERR_ERR == heap_max_insert (h, A[idx].num, NULL, &A[idx].idx))
         fprintf (stderr, "error adding heap element\n");
      //heap_dump(h);
   }
   //heap_build (h);
   heap_dump (h);
   heap_graphviz_description (h, "heap-tc_heap_max_main-i.lst");
   for (idx = 0; idx < length; idx++)
   {
      fprintf (stderr, "BEFORE -- A[idx].num = %lu A[idx].idx=%lu\n", A[idx].num, A[idx].idx);
   }
   
   heap_increase_key (h, A[9].idx, 216);
   A[9].num = 216;
   
   //heap_increase_key (h, A[9].idx, 5);
   //A[9].num = 5;
   //heap_increase_key (h, A[0].idx, 20);
//	  A[0].num = 20;

   heap_dump (h);
   heap_graphviz_description (h, "heap-tc_heap_max_main-ii.lst");

   for (idx = 0; idx < length; idx++)
   {
      fprintf (stderr, "AFTER -- A[idx].num = %lu A[idx].idx=%lu\n", A[idx].num, A[idx].idx);
   }

   return;
   
}


/**
 * @brief test heap API
 *
 */
void tc_heap_min_main (void)
{
   /*
   TC_HEAP_DATA_SET A[] = {{12,0},
                           {11,0},
                           {10,0},
                           {9,0},
                           {6,0},
                           {7,0},
                           {8,0},
                           {5,0},
                           {1,0},
                           {4,0},
                           {14,0}};
   */                           
   TC_HEAP_DATA_SET A[] = {{16, 0},
                            {4, 0},
                            {10, 0},
                            {14, 0},
                            {7, 0},
                            {9, 0},
                            {3, 0},
                            {2, 0},
                            {8, 0}/*,
                            {1, 0}*/};

   HEAP_T* h;
   unsigned int length;
   unsigned int idx;
   
   h = heap_create (DS_HEAP_MIN, 14);
   length = sizeof (A)/sizeof (TC_HEAP_DATA_SET);
   for (idx = 0; idx < length; idx++)
   {
      if (HEAP_ERR_ERR == heap_min_insert (h, A[idx].num, NULL, &A[idx].idx))
         fprintf (stderr, "error adding heap element\n");
   }
   //heap_build (h);
   heap_dump (h);
   heap_graphviz_description (h, "heap-tc_heap_min_main-i.lst");

   for (idx = 0; idx < length; idx++)
   {
      fprintf (stderr, "BEFORE -- A[idx].num = %lu A[idx].idx=%lu\n", A[idx].num, A[idx].idx);
   }

   heap_decrease_key (h, A[0].idx, 1); /* Changing the key value 16 to 1 */
   A[0].num = 1;
   heap_dump (h);
   heap_graphviz_description (h, "heap-tc_heap_min_main-ii.lst");

   for (idx = 0; idx < length; idx++)
   {
      fprintf (stderr, "AFTER -- A[idx].num = %lu A[idx].idx=%lu\n", A[idx].num, A[idx].idx);
   }
   
   return;

}


/**
 * @brief test heap API
 *
 */
void tc_heap_min_ii_main (void)
{
/*
   TC_HEAP_DATA_SET A[] = {{12,0},
                           {11,0},
                           {10,0},
                           {9,0},
                           {6,0},
                           {7,0},
                           {8,0},
                           {5,0},
                           {1,0},
                           {4,0},
                           {14,0}};
*/                           
   TC_HEAP_DATA_SET A[] = {{16, 0},
                            {4, 0},
                            {10, 0},
                            {14, 0},
                            {7, 0},
                            {9, 0},
                            {3, 0},
                            {2, 0},
                            {8, 0},
                            {1, 0}};

   HEAP_T* h;
   unsigned int length;
   unsigned int idx;
   void* p;
   unsigned long key;
   char filename[128];

   h = heap_create (DS_HEAP_MIN, 14);
   length = sizeof (A)/sizeof (TC_HEAP_DATA_SET);
   for (idx = 0; idx < length; idx++)
   {
      if (HEAP_ERR_ERR == heap_min_insert (h, A[idx].num, NULL, &A[idx].idx))
         fprintf (stderr, "error adding heap element\n");
   }
   heap_dump (h);
   heap_graphviz_description (h, "heap-tc_heap_min_ii_main-i.lst");

   for (idx = 0; idx < length; idx++)
   {
      fprintf (stderr, "FINAL -- A[idx].num = %lu A[idx].idx=%lu\n", A[idx].num, A[idx].idx);
   }

   idx=0;
   while (HEAP_SIZE(h))
   {
      heap_extract_min (h, &p, &key);
      snprintf (filename, 128, "heap-tc_heap_min_ii_main-%d.lst", idx);
      heap_graphviz_description (h, filename);
      fprintf (stderr, "FINAL == %lu\n", key);
      idx++;
   }
   return;
}
