/*  Copyright 2010 Gaurav Mathur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

#include <ll.h>
#include <queue.h>

/*******************************************************************************
* queue_dequeue_vp - create a queue of void* data elements
*
* ARGUMENTS:
*  <size> - size of queue
*            
* RETURNS: 
*/

TRUTH_E queue_is_empty
(
QUEUE_T* q
)
{
   if (0 == q->cur_size)
   {
      return DS_YES;
   }
   return DS_NO;
}

/*******************************************************************************
* queue_dequeue_vp - create a queue of void* data elements
*
* ARGUMENTS:
*  <size> - size of queue
*            
* RETURNS: 
*/

TRUTH_E queue_is_full
(
QUEUE_T* q
)
{
   if (q->max_size == q->cur_size)
   {
      return DS_YES;
   }
   return DS_NO;
}

/*******************************************************************************
* queue_dequeue_vp - create a queue of void* data elements
*
* ARGUMENTS:
*  <size> - size of queue
*            
* RETURNS: 
*/

void* queue_dequeue_vp
(
QUEUE_T* q,
QUEUE_ERR_E* err
)
{
   SLL_NODE* node;

   if (0 == q->cur_size)
   {
      *err = Q_ERR_UNDERFLOW;
      return NULL;
   }
   
   node = sll_remove_first (q->q);
   if (NULL == node)
   {
      *err = Q_ERR_ERR;
      return NULL;
   }
   q->cur_size--;
   return node->FVPDATA;
}

/*******************************************************************************
* queue_enqueue_vp - create a queue of void* data elements
*
* ARGUMENTS:
*  <size> - size of queue
*            
* RETURNS: 
*/

QUEUE_ERR_E queue_enqueue_vp
(
QUEUE_T* q,
void* data
)
{
   if (q->cur_size == q->max_size)
      return Q_ERR_OVERFLOW;

   if (ERR_LL_OK != sll_insert (q->q, data))
   {
      return Q_ERR_ERR;
   }
   q->cur_size++;
   
   return Q_ERR_OK;
}

/*******************************************************************************
* queue_create - create a queue of integer data elements
*
* ARGUMENTS:
*  <size> - size of queue
*            
* RETURNS: 
*/

QUEUE_T* queue_create
(
NODE_DATA_TYPE_E data_type,
unsigned long size
)
{
   QUEUE_T* q;

   if (NULL == (q = malloc (sizeof (QUEUE_T))))
      return NULL;
   
   if (NULL == (q->q = sll_new (data_type, NULL, NULL, NULL)))
   {
      free (q);
      return NULL;
   }
   q->max_size = size;
   q->cur_size = 0;
   return q;
}


/*******************************************************************************
 * queue_new - create a new queue
 *
 * RETURNS: 
 */

int queue_main (int argc, char** argv)
    {
       int val = 0;
       int *data = malloc (10 * sizeof (int));
       int *tmp;
       QUEUE_ERR_E err;
       QUEUE_T* q;
       
       tmp = data;
       for (val = 100; val <= 110; val++)
       {
          *tmp = val;
          tmp++;
       }

       q = queue_create (VPDATA, 10);
       tmp = data;
       
       for (val = 100; val < 110; val++)
       {
          fprintf (stderr, "inserting %d... ", *tmp);
          queue_enqueue_vp (q, tmp++);
          fprintf (stderr, "depth is now=%lu\n", QUEUE_DEPTH(q));
       }

       err = queue_enqueue_vp (q, tmp);
       printf ("\nerr=%d, expected=%d\n", err, Q_ERR_OVERFLOW);
       
       for (val = 100; val < 110; val++)
       {
          printf ("%d ", *(int *)queue_dequeue_vp (q, &err));
       }

       queue_dequeue_vp (q, &err);
       printf ("\nerr=%d, expected=%d\n", err, Q_ERR_UNDERFLOW);
       return 0;
    }

