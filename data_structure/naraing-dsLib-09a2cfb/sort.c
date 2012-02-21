/**
 * sorting algorithms
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

/**
 * @brief heapsort
 *
 * This routine implements the heapsort sorting algorithm
 *
 * HEAPSORT(A)
 * 1 BUILD-MAX-HEAP(A)
 * 2 for i <- length[A] downto 2
 * 3    do exchange A[1] <-> A[i]
 * 4       heap-size[A] <- heap-size[A] - 1
 * 5       MAX-HEAPIFY(A, 1)
 */
int heapsort (HEAP_T* h)
{
   int i;
   unsigned long shs = HEAP_SIZE(h); /* save heap size */
   
   if (IS_NOT_MAX_HEAP(h))
      return -1;

   heap_build (h); /* todo: skip building heap if already heapified */
   for (i = HEAP_LEN(h)-1; i >= 1; i--)
   {
      HEAP_SWAP_NODES(0,i);
      HEAP_SIZE(h) = HEAP_SIZE(h) - 1;
      heap_max_heapify (h, 0);
   }
   HEAP_SIZE(h) = shs; /* restore heap size */
   return 0;
}

/**
 * @brief insertion sort
 *
 * This routine implements the insertion sort algorithm
 *
 * @param[in] arr array of integers to be sorted (R/W)
 * @param[in] num number of elements in the array
 * @return int 0 always
 */
int isort_i (int* arr, size_t num)
{
   size_t i, j;
   int key;
   int ele;

   for (j = 0; j < num; j++)
   {
      key = *(arr+j);
      i = j - 1;
      ele = *(arr+i);
      while ((i > 0) && (key < ele))
         {
            *(arr+i+1) = ele;
            i--;
            ele = *(arr+i);
         }
      *(arr+i+1) = key;
   }
   return 0;
}

