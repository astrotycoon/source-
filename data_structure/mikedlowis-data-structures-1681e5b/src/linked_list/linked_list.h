/******************************************************************************
 *  Copyright (C) 2011  Michael D. Lowis
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "common.h"

#define   PTR_TYPE  void *

typedef struct LinkedList
{
	PTR_TYPE contents;
	struct LinkedList * next;
} LinkedList_T;

/**
 * @brief	Creates a new linked list node with the supplied value.
 * 
 * Allocates a new node on the heap and populates the node contents with the 
 * supplied contents pointer.
 *
 * @param	contents    The contents of the newly created node.
 *
 * @return	A pointer to the newly created node.
 * */
LinkedList_T* LL_New( PTR_TYPE contents );

/**
 * @brief	Finds and returns the last node in the supplied linked list.
 *
 * @param	list    The linked list to search.
 *
 * @return	Pointer to the last node in the supplied list.
 * */
LinkedList_T* LL_Last(LinkedList_T* list);

/**
 * @brief	Return the node at the specified index in a linked list.
 *
 * Loops through the linked list and returns the node in the list at the
 * specified index. Returns NULL if the index is out of range.
 *
 * @param	list    The list to search for the supplied index.
 * @param	index   The index of the node to return.
 *
 * @return	A pointer to the node and the supplied index, NULL if out of range.
 * */
LinkedList_T* LL_Get(LinkedList_T* list, int index);

/**
 * @brief	Adds a new node to an existing linked list.
 *
 * @param	list
 * @param	contents
 * */
void LL_Add( LinkedList_T* list, PTR_TYPE contents );

/**
 * @brief	Inserts a new node in a linked list at the specified index.
 *
 * @param	list
 * @param	index
 * @param	contents
 *
 * @return	Pointer to the newly inserted node, NULL if index is out of range.
 * */
LinkedList_T* LL_Insert( LinkedList_T* list, int index, PTR_TYPE contents);

/**
 * @brief	Deletes a node from the supplied list.
 *
 * Deletes the node found at the supplied index from the supplied list and frees
 * the memory used by the node and its contents.
 *
 * @param	list
 * @param	index
 * @param   free_contents Whether or not to also free the contents of the node.
 * */
void LL_Delete( LinkedList_T* list, int index, BOOL free_contents);

/**
 * @brief	Frees all memory used by a linked list.
 *
 * Loops through the supplied list and frees all nodes. Also frees contents if
 * free_contents is passed TRUE. This is to avoid trying to free memory 
 * allocated on the stack.
 *
 * @param	list          The list to be freed.
 * @param   free_contents Whether or not to also free the contents of each node.
 * */
void LL_Free( LinkedList_T* list, BOOL free_contents);

/**
 * @brief Returns the number of elements in the list.
 * 
 * Loops through the supplied list and returns a count of the number of elements
 * contained in the list.
 *
 * @param list        The list to be counted.
 *
 * @return The number of elements in the list.
 **/
U32 LL_Length(LinkedList_T* list);

#endif
