/*
Copyright (c) 2011, Coleman Stavish
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
  * Neither the name of Coleman Stavish nor the
	names of contributors may be used to endorse or promote products
	derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COLEMAN STAVISH BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CS_LINKED_LIST_H
#define CS_LINKED_LIST_H

#include <stdint.h>
#include <stddef.h>

struct cs_sll_node {
    void *data;
    struct cs_sll_node *next;
};

struct cs_dll_node {
    void *data;
    struct cs_dll_node *next, *prev;
};

struct cs_dll_head {
    size_t size;
    struct cs_dll_node *start, *end;
    void (*cleanup)(void *);
};

struct cs_sll_head {
    size_t size;
    struct cs_sll_node *start;
    void (*cleanup)(void *);
};

typedef struct cs_sll_node cs_sll_node;
typedef struct cs_dll_node cs_dll_node;
typedef struct cs_sll_head cs_sll;
typedef struct cs_dll_head cs_dll;

/**
 * cs_sll_create | cs_dll_create - create {singly|doubly} linked list
 *
 * @param cleanup - CAN BE NULL - pointer to function called when a node is destroyed in cs_{s|d}ll_destroy()
 * @param ...           - NULL-terminated list of elements of any pointer type
 *
 * @return - a reference to a newly allocated cs_{s|d}ll struct populated with the specified elements
 *
 */
cs_sll *cs_sll_create(void (*cleanup)(void *), ...);
cs_dll *cs_dll_create(void (*cleanup)(void *), ...);


/**
 * cs_sll_get | cs_dll_get - get element
 *
 * @param list  - the list whence the element at the specified index shall be removed
 * @param index - the location of the element to be removed
 *
 * @return - the value stored in the element at the specified index, or NULL, if (not iff) no element exists at the specified index
 *
 */
void *cs_sll_get(cs_sll *list, size_t index);
void *cs_dll_get(cs_dll *list, size_t index);


/**
 * cs_sll_ins | cs_dll_ins - insert element
 *
 * NOTE: if an element x exists at index y, and a new element z is to be inserted at y, then x will be 'shifted'
 *              - such that z->next == x
 *
 * @param list  - the list into which the specified value will be inserted at the specified index
 * @param index - the location of the element to be inserted
 *
 * @return - 0 upon failure, 1 upon success
 *
 */
uint8_t cs_sll_ins(cs_sll *list, void *data, size_t index);
uint8_t cs_dll_ins(cs_dll *list, void *data, size_t index);


/**
 * cs_sll_swap | cs_dll_swap - swap data from index x with index y
 *
 * @return - 0 upon failure, 1 upon success
 *
 */
uint8_t cs_sll_swap(cs_sll *list, size_t x, size_t y);
uint8_t cs_dll_swap(cs_dll *list, size_t x, size_t y);

/**
 * cs_sll_set | cs_dll_set - set the specified data at the specified index
 *
 * @return - 0 upon failure, 1 upon success
 *
 * NOTE: these functions cannot be used to insert new elements
 *
 */
uint8_t cs_sll_set(cs_sll *list, void *data, size_t index);
uint8_t cs_dll_set(cs_dll *list, void *data, size_t index);


/**
 * cs_sll_del | cs_dll_del - remove element
 *
 * NOTE: remove y from (x -> y -> z) yields (x -> z)
 *
 * @param list  - the list whence the data at the specified index shall be removed
 * @param index - the location of the element to be removed
 *
 * @return - data stored in the removed element, or NULL, if (not iff) no element exists at the specified index
 *
 */
void *cs_sll_del(cs_sll *list, size_t index);
void *cs_dll_del(cs_dll *list, size_t index);


/**
 * cs_sll_app | cs_dll_app - append element
 *
 * @param list  - the list to which the specified data will be appeneded
 * @param data  - the data to be appended
 *
 * @return - 0 upon failure, 1 upon success
 *
 */
uint8_t cs_sll_app(cs_sll *list, void *data);
uint8_t cs_dll_app(cs_dll *list, void *data);

/**
 * cs_sll_destroy | cs_dll_destroy - destroy a list and its elements
 *
 * NOTE: if list->cleanup is not NULL, it will be called with each element's data
 *
 * @param list  - the list to be destroyed
 *
 */
void cs_sll_destroy(cs_sll *list);
void cs_dll_destroy(cs_dll *list);

#endif