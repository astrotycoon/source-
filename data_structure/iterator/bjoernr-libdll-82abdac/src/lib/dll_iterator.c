/*
* Copyright (c) 2008, Björn Rehm (bjoern@shugaa.de)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "dll_list.h"
#include "dll_list_prv.h"

/* ######################################################################### */
/*                            TODO / Notes                                   */
/* ######################################################################### */

/* ######################################################################### */
/*                            Types & Defines                                */
/* ######################################################################### */

#define DLL_ITERATOR_INIT       (1<<0)              

/* ######################################################################### */
/*                           Private interface (Module)                      */
/* ######################################################################### */

/* ######################################################################### */
/*                           Implementation                                  */
/* ######################################################################### */

int dll_iterator_init(dll_iterator_t *iterator, dll_list_t *list)
{
        if (!iterator)
                return EDLLINV;
        if (!list)
                return EDLLINV; 

        /* Initialize the iterator */
        iterator->flags = 0;
        iterator->list = list;
        iterator->item = NULL;

        return EDLLOK;
}

int dll_iterator_next(dll_iterator_t *iterator, void **data, size_t *datasize)
{
	int ret = EDLLOK;

    if (!iterator)
		return EDLLINV;
    if (!data)
        return EDLLINV;

    if ((iterator->flags & DLL_ITERATOR_INIT) < DLL_ITERATOR_INIT) 
	{
		iterator->flags = DLL_ITERATOR_INIT;
        iterator->item = iterator->list->first;
    } 
	else 
	{
		if (iterator->item == iterator->list->last) 
		{
			iterator->item = iterator->list->first;
				ret = EDLLTILT;
		} 
		else
		{
			if (iterator->item != NULL) 
			{
				iterator->item = iterator->item->next;
            }
		}	
    }

    if (iterator->item == NULL)
            return EDLLERROR;

    *data = iterator->item->data;
	if (datasize != NULL)
		*datasize = iterator->item->datasize;

    return ret;
}

int dll_iterator_prev(dll_iterator_t *iterator, void **data, size_t *datasize)
{
        int ret = EDLLOK;

        if (!iterator)
                return EDLLINV;
        if (!data)
                return EDLLINV;

        if ((iterator->flags & DLL_ITERATOR_INIT) < DLL_ITERATOR_INIT) {
                iterator->flags = DLL_ITERATOR_INIT;
                iterator->item = iterator->list->last;
        } else
        {
                if (iterator->item == iterator->list->first) {
                        iterator->item = iterator->list->last;
                        ret = EDLLTILT;
                } else if (iterator->item != NULL) {
                        iterator->item = iterator->item->prev;
                }
        }

        if (iterator->item == NULL)
                return EDLLERROR;

        *data = iterator->item->data;
        if (datasize != NULL)
            *datasize = iterator->item->datasize;

        return ret;
}

