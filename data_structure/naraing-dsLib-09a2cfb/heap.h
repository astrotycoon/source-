/** @file heap.h
 * 
 * heap data structure and operations headers
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

#include <limits.h>

typedef enum {DS_HEAP_MAX=1, DS_HEAP_MIN=2} DS_HEAP_TYPE_E;

typedef struct ds_heap_node
    {
    unsigned long key;
    void* data;
    unsigned long* i;
    } DS_HEAP_NODE_T;

typedef struct ds_heap_t {
   unsigned long length;
   unsigned long avail_index;
   unsigned long heap_size;
   DS_HEAP_NODE_T** nodes;
   DS_HEAP_TYPE_E type;
   /* char was_heapified; */ /* elements could be just added to heap and not heapified */
} HEAP_T;

/* heap operation result code  */
typedef enum {
   HEAP_ERR_ERROR_HIGH = -512,   /* fencepost */
   HEAP_ERR_INVALID_ARGS,
   HEAP_ERR_MALLOC_FAIL,
   HEAP_ERR_WRONG_TYPE,
   HEAP_ERR_UNDERFLOW,
   HEAP_ERR_INVALID_KEY,
   HEAP_ERR_LARGER_KEY,
   HEAP_ERR_SMALLER_KEY,
   HEAP_ERR_ERR = -1,
   HEAP_ERR_OK = 0,
   HEAP_ERR_ITERATOR_DONE = 1
} HEAP_ERR_E;

#define HEAP_PARENT(I)  ((I)?((I-1)/2):0)
#define HEAP_LEFT(I)    (2*(I) + 1)
#define HEAP_RIGHT(I)   (2*(I) + 2)

#define HEAP_KEY(H, I)          ((H)->nodes[I]->key)
#define HEAP_DATA(H, I)         ((H)->nodes[I]->data)
#define HEAP_I(H, I)            ((H)->nodes[I]->i)
#define HEAP_I_VAL(H, I)        (*((H)->nodes[I]->i))
#define HEAP_SIZE(H)            ((H)->heap_size)
#define HEAP_LEN(H)             ((H)->length)

#define IS_MAX_HEAP(H)          ((H)->type == DS_HEAP_MAX)
#define IS_NOT_MAX_HEAP(H)      ((H)->type != DS_HEAP_MAX)
#define IS_MIN_HEAP(H)          ((H)->type == DS_HEAP_MIN)
#define IS_NOT_MIN_HEAP(H)      ((H)->type != DS_HEAP_MIN)

#define HEAP_NIL_KEY    ULONG_MAX

#define HEAP_SWAP_NODES(N1,N2)                                          \
   do {                                                                 \
      void* tmp_data;                                                   \
      unsigned long tmp_key;                                            \
      unsigned long* tmp_i;                                             \
                                                                        \
      tmp_key = h->nodes[N2]->key;                                      \
      tmp_data = h->nodes[N2]->data;                                    \
      tmp_i = h->nodes[N2]->i;                                          \
      h->nodes[N2]->key = h->nodes[N1]->key;                            \
      h->nodes[N2]->data = h->nodes[N1]->data;                          \
      h->nodes[N2]->i = h->nodes[N1]->i;                                \
      h->nodes[N1]->key = tmp_key;                                      \
      h->nodes[N1]->data = tmp_data;                                    \
      h->nodes[N1]->i = tmp_i;                                          \
      if (h->nodes[N1]->i)                                              \
         *h->nodes[N1]->i = N1;                                         \
      if (h->nodes[N2]->i)                                              \
         *h->nodes[N2]->i = N2;                                         \
   }while (0)

HEAP_T* heap_create (DS_HEAP_TYPE_E type, unsigned long length);
HEAP_ERR_E heap_build (HEAP_T* h);
HEAP_ERR_E heap_iter (HEAP_T* h, void** data, unsigned long* key, unsigned long* ctx);
HEAP_ERR_E heap_max_heapify (HEAP_T* h, unsigned long i);
HEAP_ERR_E heap_min_heapify (HEAP_T* h, unsigned long i);
HEAP_ERR_E heap_extract_min (HEAP_T* h, void** data, unsigned long* key);
HEAP_ERR_E heap_extract_max (HEAP_T* h, void** data, unsigned long* key);
HEAP_ERR_E heap_minimum (HEAP_T* h, void** data, unsigned long* key);
HEAP_ERR_E heap_maximum (HEAP_T* h, void** data, unsigned long* key);
HEAP_ERR_E heap_decrease_key (HEAP_T* h, unsigned long i, unsigned long key);
HEAP_ERR_E heap_increase_key (HEAP_T* h, unsigned long i, unsigned long key);
HEAP_ERR_E heap_max_insert (HEAP_T* h, unsigned long key, void* data, unsigned long* i);
HEAP_ERR_E heap_min_insert (HEAP_T* h, unsigned long key, void* data, unsigned long* i);
HEAP_ERR_E heap_graphviz_description (HEAP_T* h, char* filename);
void heap_dump (HEAP_T* h);
