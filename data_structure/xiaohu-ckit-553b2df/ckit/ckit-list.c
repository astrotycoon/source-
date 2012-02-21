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
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ckit-list.h"

ckit_list_t*
ckit_list_append (ckit_list_t *list, void *data)
{
  ckit_list_t *node, *last;

  node = calloc (1, sizeof (ckit_list_t));
  node->data = data;

  last = ckit_list_last (list);
  if (!last)
    return node;

  last->next = node;
  node->prev = last;

  return list;
}


ckit_list_t*
ckit_list_prepend (ckit_list_t *list, void *data)
{
  ckit_list_t *node;

  node = calloc (1, sizeof (ckit_list_t));
  node->data = data;
  node->next = list;

  if (list)
    {
      node->prev = list->prev;
      if (list->prev)
	list->prev->next = node;
      list->prev = node;
    }
  return node;
}


ckit_list_t*
ckit_list_insert (ckit_list_t *list, void *data, long index)
{
  ckit_list_t *tmp, *node;

  if (index < 0)
    return ckit_list_append (list, data);
  if (index == 0)
    return ckit_list_prepend (list, data);

  tmp = ckit_list_nth (list, index);
  if (!tmp)
    return ckit_list_append (list, data);

  node = calloc (1, sizeof (ckit_list_t));
  node->data = data;
  node->prev = tmp->prev;
  if (tmp->prev)
    tmp->prev->next = node;
  node->next = tmp;
  tmp->prev = node;

  if (tmp == list)
    return node;
  else
    return list;
}


ckit_list_t*
ckit_list_remove (ckit_list_t *list, void *data)
{
  ckit_list_t *tmp = list;
  while (tmp)
    {
      if (tmp->data == data)
	{
	  if (tmp->prev)
	    tmp->prev->next = tmp->next;
	  if (tmp->next)
	    tmp->next->prev = tmp->prev;

	  if (tmp == list)
	    list = list->next;

	  free (tmp);
	  break;
	}
      tmp = tmp->next;
    }
  return list;
}


ckit_list_t*
ckit_list_remove_all (ckit_list_t *list, void *data)
{
  ckit_list_t *next, *tmp = list;
  while (tmp)
    {
      if (tmp->data == data)
	{
	  next = tmp->next;

	  if (tmp->prev)
	    tmp->prev->next = tmp->next;
	  if (tmp->next)
	    tmp->next->prev = tmp->prev;

	  if (tmp == list)
	    list = list->next;

	  free (tmp);
	  tmp = next;
	  continue;
	}
      tmp = tmp->next;
    }
  return list;
}


ckit_list_t*
ckit_list_remove_index (ckit_list_t *list, long index,
			ckit_free_f data_free_func)
{
  ckit_list_t *node = ckit_list_nth (list, index);
  if (!node)
    return list;

  if (node->next)
    node->next->prev = node->prev;
  if (node->prev)
    node->prev->next = node->next;

  if (node == list)
    list = list->next;

  if (data_free_func)
    data_free_func (node->data);

  free (node);

  return list;
}


ckit_list_t*
ckit_list_remove_node (ckit_list_t *list, ckit_list_t *link)
{
  if (link)
    {
      if (link->prev)
	link->prev->next = link->next;
      if (link->next)
	link->next->prev = link->prev;
      if (link == list)
	list = list->next;

      link->prev = NULL;
      link->next = NULL;
    }
  return list;
}


ckit_list_t*
ckit_list_delete_node (ckit_list_t *list, ckit_list_t *link)
{
  list = ckit_list_remove_node (list, link);
  ckit_list_free (link);
  return list;
}


void
ckit_list_free (ckit_list_t *list)
{
  while (list)
    {
      ckit_list_t *node = list->next;
      free (list);
      list = node;
    }
}


void
ckit_list_free_with_data (ckit_list_t *list, ckit_free_f data_free_func)
{
  while (list)
    {
      ckit_list_t *node = list->next;
      if (data_free_func)
	data_free_func (list->data);
      free (list);
      list = node;
    }
}


ckit_list_t*
ckit_list_first (ckit_list_t *list)
{
  if (list)
    {
      while (list->prev)
	list = list->prev;
    }
  return list;
}


ckit_list_t*
ckit_list_last (ckit_list_t *list)
{
  if (list)
    {
      while (list->next)
	list = list->next;
    }
  return list;
}


ckit_list_t*
ckit_list_nth (ckit_list_t *list, long index)
{
  if (index < 0)
    return ckit_list_last (list);

  while ((index-- > 0) && list)
    list = list->next;

  return list;
}


void*
ckit_list_nth_data (ckit_list_t *list, long index)
{
  ckit_list_t *node = ckit_list_nth (list, index);
  if (node)
    return node->data;
  else
    return NULL;
}


long
ckit_list_index (ckit_list_t *list, void *data)
{
  long index = 0;

  while (list)
    {
      if (list->data == data)
	return index;
      list = list->next;
      index++;
    }
  return -1;
}


long
ckit_list_position (ckit_list_t *list, ckit_list_t *link)
{
  long pos = 0;
  while (list)
    {
      if (list == link)
	return pos;
      pos++;
      list = list->next;
    }
  return -1;
}


ckit_list_t*
ckit_list_find (ckit_list_t *list, void *data)
{
  while (list)
    {
      if (list->data == data)
	return list;
      list = list->next;
    }
  return NULL;
}


ckit_list_t*
ckit_list_find_custom (ckit_list_t *list, void *data, ckit_compare_f func)
{
  while (list)
    {
      if (func (list->data, data) == 0)
	return list;
      list = list->next;
    }
  return NULL;
}


ulong_t
ckit_list_length (ckit_list_t *list)
{
  ulong_t length = 0;
  while (list)
    {
      list = list->next;
      length++;
    }
  return length;
}


ckit_list_t*
ckit_list_reverse (ckit_list_t *list)
{
  ckit_list_t *last = NULL;
  while (list)
    {
      last = list;
      list = last->next;
      last->next = last->prev;
      last->prev = list;
    }
  return last;
}


void
ckit_list_foreach (ckit_list_t *list, ckit_foreach_f func, void *userdata)
{
  while (list)
    {
      ckit_list_t *next = list->next;
      if ((*func) (list->data, userdata) == 0)
	break;
      list = next;
    }
}


ckit_list_t*
ckit_list_concat (ckit_list_t *list1, ckit_list_t *list2)
{
  if (list2)
    {
      ckit_list_t *tmp = ckit_list_last (list1);
      if (tmp)
	tmp->next = list2;
      else
	list1 = list2;
      list2->prev = tmp;
    }
  return list1;
}


ckit_list_t*
ckit_list_copy (ckit_list_t *list)
{
  ckit_list_t *list2 = NULL;
  if (list)
    {
      ckit_list_t *last;
      list2 = calloc (1, sizeof (ckit_list_t));
      list2->data = list->data;
      last = list2;
      list = list->next;
      while (list)
	{
	  last->next = calloc (1, sizeof (ckit_list_t));
	  last->next->prev = last;
	  last = last->next;
	  last->data = list->data;
	  list = list->next;
	}
    }
  return list2;
}


ckit_list_t*
ckit_list_sort (ckit_list_t *list, ckit_compare_f func)
{
#warning "unimplement ckit_list_sort"
  return NULL;
}
