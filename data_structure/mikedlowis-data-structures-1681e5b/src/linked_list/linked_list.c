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
#include "linked_list.h"
#include <stdlib.h>

/******************************************************************************
 * Public Functions
 ******************************************************************************/
LinkedList_T* LL_New( PTR_TYPE contents )
{
	LinkedList_T* list = (LinkedList_T*)malloc( sizeof(LinkedList_T) );
	list->contents = contents;
	list->next = NULL;
	return list;
}

LinkedList_T* LL_Last( LinkedList_T* list )
{
	LinkedList_T* node = list;
	while((node != NULL) && (node->next != NULL))
	{
		node = node->next;
	}
	return node;
}

LinkedList_T* LL_Get( LinkedList_T* list, int index )
{
	int current = 0;
	LinkedList_T* node = list;
	LinkedList_T* indexed_node = NULL;
	while ((node != NULL))
	{
		if ( current == index )
		{
			indexed_node = node;
			break;
		}
		node = node->next;
		current++;
	}
	return indexed_node;
}

void LL_Add( LinkedList_T* list, PTR_TYPE contents )
{
	LinkedList_T* node = LL_Last( list );
	node->next = LL_New( contents );
}

LinkedList_T* LL_Insert( LinkedList_T* list, int index, PTR_TYPE contents )
{
	int req_index = ((index-1) < 0) ? 0 : index-1;
	LinkedList_T* node = LL_Get( list, req_index );
	if(node != NULL)
	{
		LinkedList_T* next_next = node->next;
		node->next = LL_New( contents );
		node->next->next = next_next;
		node = node->next;
	}
	return node;
}

void LL_Delete( LinkedList_T* list, int index, BOOL free_contents)
{
	LinkedList_T* node = LL_Get( list, (index-1));
	if((node != NULL) && (node->next != NULL))
	{
		LinkedList_T* node_to_delete = node->next;
		node->next = node_to_delete->next;
		if (free_contents)
		{
			free(node_to_delete->contents);
		}
		free(node_to_delete);
	}
}

void LL_Free( LinkedList_T* list, BOOL free_contents)
{
	LinkedList_T* node = list;
	while( node != NULL )
	{
		LinkedList_T* next = node->next;
		if (free_contents)
			{
			free(node->contents);
			}
		free(node);
		node = next;
	}
}

U32 LL_Length(LinkedList_T* list)
{
	U32 length = 0;
	LinkedList_T* item = list;
	for ( item = list; item != NULL; item = item->next )
	{
		length++;	
	}
	return length;
}

