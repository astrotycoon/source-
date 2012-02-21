/* CKit: Data structure and arithmetic implement in C
 * Copyright (C) 2011 Wu Xiaohu, All rights reserved.
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
#if !defined (__CKIT_H_INSIDE__) && !defined (CKIT_COMPILATION)
# error "Only <ckit/ckit.h> can be included directly."
#endif

#ifndef CKIT_LIST_H
#define CKIT_LIST_H

#include <ckit/ckit-types.h>

CKIT_BEGIN_DECLS

typedef struct _ckit_list  ckit_list_t;
struct _ckit_list
{
  ckit_list_t *next, *prev;
  void        *data;
};

ckit_list_t* ckit_list_append (ckit_list_t *list, void *data) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_prepend (ckit_list_t *list, void *data) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_insert (ckit_list_t *list, void *data, long index) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_remove (ckit_list_t *list, void *data) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_remove_all (ckit_list_t *list, void *data) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_remove_index (ckit_list_t *list, long index, ckit_free_f data_free_func) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_remove_node (ckit_list_t *list, ckit_list_t *link) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_delete_node (ckit_list_t *list, ckit_list_t *link) CKIT_WARN_UNUSED_RESULT;
void ckit_list_free (ckit_list_t *list);
void ckit_list_free_with_data (ckit_list_t *list, ckit_free_f data_free_func);
ckit_list_t* ckit_list_first (ckit_list_t *list) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_last (ckit_list_t *list) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_nth (ckit_list_t *list, long index);
void* ckit_list_nth_data (ckit_list_t *list, long index);
long ckit_list_index (ckit_list_t *list, void *data);
long ckit_list_position (ckit_list_t *list, ckit_list_t *link);
ckit_list_t* ckit_list_find (ckit_list_t *list, void *data);
ckit_list_t* ckit_list_find_custom (ckit_list_t *list, void *data, ckit_compare_f func);
ulong_t ckit_list_length (ckit_list_t *list);
ckit_list_t* ckit_list_reverse (ckit_list_t *list) CKIT_WARN_UNUSED_RESULT;
void ckit_list_foreach (ckit_list_t *list, ckit_foreach_f func, void *userdata);
ckit_list_t* ckit_list_concat (ckit_list_t *list1, ckit_list_t *list2) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_copy (ckit_list_t *list) CKIT_WARN_UNUSED_RESULT;
ckit_list_t* ckit_list_sort (ckit_list_t *list, ckit_compare_f func) CKIT_WARN_UNUSED_RESULT;

#define ckit_list_prev(list)  ((list) ? (((ckit_list_t*) list)->prev) : NULL)
#define ckit_list_next(list)  ((list) ? (((ckit_list_t*) list)->next) : NULL)
#define ckit_list_data(list)  ((list) ? (((ckit_list_t*) list)->data) : NULL)

CKIT_END_DECLS

#endif /* CKIT_LIST_H */
