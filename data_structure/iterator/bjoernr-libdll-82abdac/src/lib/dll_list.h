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

/** @file dll_list.h
 *
 * @brief Public library interface
 *
 * */

/** @mainpage A small and portable, yet feature rich doubly linked list implementation
 *
 * Installation instructions can be found in INSTALL.txt in the software
 * package's root directory.
 *
 * Feedback to bjoern@shugaa.de is always highly appreciated.
 */

#ifndef _DLL_LIST_H
#define _DLL_LIST_H

#include <stdio.h>

/* ######################################################################### */
/*                            TODO / Notes                                   */
/* ######################################################################### */

/* ######################################################################### */
/*                            Types & Defines                                */
/* ######################################################################### */

#define EDLLERROR   (0)   /* General error */
#define EDLLOK      (1)   /* Success */
#define EDLLTILT    (2)   /* Iterator turnaround */
#define EDLLNOMEM   (3)   /* Unable to allocate enough memory */
#define EDLLINV     (4)   /* Invalid argument */

/** List item type */
typedef struct dll_item dll_item_t;

/** List instance type */
typedef struct dll_list dll_list_t;

/** List iterator type */
typedef struct dll_iterator dll_iterator_t;

struct dll_list
{
        unsigned int count;
        dll_item_t *first;
        dll_item_t *last;
};

struct dll_iterator
{
        int flags;
        dll_item_t *item;
        dll_list_t *list;
};

/** Comparator function prototype */
typedef int(*dll_fctcompare_t)(const void*, const void*);

/* ######################################################################### */
/*                            Public interface                               */
/* ######################################################################### */

/** Initialize a doubly-linked list instance
 *
 * @param list       Pointer to a dll_list_t to be initialized
 *
 * @return EDLLOK    No errors occured
 * @return EDLLERROR Something went wrong
 */
int dll_init(dll_list_t *list);

/** Clear all items from the linked list
 *
 * This will free each dll_item_t container in the list and the client data
 * contained in it. It is vital to use this function for cleaning up lists when
 * you're done using them, otherwise you will leak memory.
 *
 * @param list       Pointer to the list
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_clear(dll_list_t *list);

/** Append an item to the end of the list
 *
 * @param list       Pointer to the list
 * @param data       Where to store the reference to the allocated memory
 * @param datasize   Size of memory to be allocated for this item's data    
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_append(dll_list_t *list, void **data, size_t datasize);

/** Extend a list with another one
 *
 * @param list       Pointer to the list to be extended
 * @param lext       Pointer to extension list
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_extend(dll_list_t *list, dll_list_t *lext);

/** Insert a new item into the list at the specified position
 *
 * @param list       Pointer to the list
 * @param data       Where to store the reference to the allocated memory
 * @param datasize   Size of memory to be allocated for this item's data   
 * @param position   Position in the list to insert the new item (starts with 0)
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_insert(dll_list_t *list, void **data, size_t datasize, unsigned int position);

/** Remove a specific item from the list
 *
 * @param list       Pointer to the list
 * @param position   Position of the item to be removed
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_remove(dll_list_t *list, unsigned int position);

/** Get an item from the list
 *
 * @param list       Pointer to the list
 * @param data       Where to store the reference to the specified item data
 * @param datasize   Size of the data BLOB
 * @param position   Position in the list of the requested item
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_get(dll_list_t *list, void **data, size_t *datasize, unsigned int position);

/** Get the current item count of the list
 *
 * @param list       Pointer to the list
 * @param count      Pointer to an unsigned int to store the count in
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_count(dll_list_t *list, unsigned int *count);

/** Make a deep copy of a list
 *
 * @param from       List to be copied from
 * @param to         List instance to be copied to (needs to be initialised and empty)
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_deepcopy(dll_list_t *from, dll_list_t *to);

/** Reverse a list
 *
 * @param list       Pointer to the list
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_reverse(dll_list_t *list);

/** Sort a doubly linked list
 * This implementation uses quicksort which is fast and in-place 
 *
 * @param list       List to be sorted
 * @param compar     Pointer to function comparing two data items
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_sort(dll_list_t *list, dll_fctcompare_t compar);

/** Returns the first occurence of the item which successfully compares to
 * 'cmpitem'
 *
 * @param list       List to be searched
 * @param compar     Pointer to function comparing two data items
 * @param cmpitem    pointer to an item to compare to        
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong / Item not found
 */
int dll_indexof(dll_list_t *list, dll_fctcompare_t compar, void *cmpitem, unsigned int *index);

/** Create a new doubly-linked list iterator instance
 *
 * Call dll_iterator_next() to move the iterator to the first list item after 
 * you have created it. Call dll_iterator_prev() to point to the last element 
 * of the list. 
 * Turnarounds are possible. If a jump from the last to the first element or 
 * vice versa occured upon a call to dll_iterator_next() or dll_iterator_prev()
 * the status code EDLLTILT is returned.
 *  
 * Using an iterator to traverse a list is a lot faster then getting each item
 * using dll_get() since it's not necessary anymore to walk from element 0 to
 * the requested element multiple times. The next element is always just a
 * reference away.
 *
 * @param iterator   Pointer to a dll_iterator_t to be initialized 
 * @param list       Pointer to dll_list_t for which the iterator is to be created
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLERROR Something went wrong
 */
int dll_iterator_init(dll_iterator_t *iterator, dll_list_t *list);

/** Move the iterator to the next position
 *
 * @param iterator   The iterator which is to be moved to the next element
 * @param data       Storage for the reference to this item's data
 * @param datasize   Size of the data BLOB
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLTILT  Iterator turnaround (jump from last to first item)
 * @return EDLLERROR Something went wrong
 */
int dll_iterator_next(dll_iterator_t *iterator, void **data, size_t *datasize);

/** Move the iterator to the previous position
 *
 * @param iterator   The iterator which is to be moved to the next element
 * @param data       Storage for the reference to this item's data
 * @param datasize   Size of the data BLOB
 *
 * @return EDLLOK    No errors occured
 * @return EDLLINV   An invalid argument has been passed
 * @return EDLLTILT  Iterator turnaround (jump from first to last item)
 * @return EDLLERROR Something went wrong
 */
int dll_iterator_prev(dll_iterator_t *iterator, void **data, size_t *datasize);

#endif /* _DLL_LIST_H */

