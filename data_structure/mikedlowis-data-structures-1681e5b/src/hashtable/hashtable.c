/******************************************************************************
 *  Copyright (C) 2011 Michael D. Lowis 
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
#include "hashtable.h"
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>

STATIC U32 Hash_HashString(String key);

/******************************************************************************
 * Private Functions
 *****************************************************************************/
U32 Hash_HashString(String key)
{
	U32 hash = 0;
	U8 i = 0;
	for (i = 0; key[i] != '\0'; i++)
	{
		hash += key[i];
	}
	return hash;	
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/
HashTable_T* Hash_New(U32 size, HashFuncPtr_T fn)
{
	U32 table_size = size * sizeof(HashNode_T*);
	HashTable_T* table = (HashTable_T*) malloc( sizeof(HashTable_T) );
	table->size = size;
	table->table = (HashNode_T**) malloc( table_size );
	table->hash_func = (fn != NULL) ? fn : (HashFuncPtr_T) Hash_HashString;
	memset(table->table, 0, table_size);
	return table;
}

void Hash_Free(HashTable_T* table)
{
	U8 i = 0;
	for (i = 0; i < table->size; i++)
	{
		HashNode_T* cur = table->table[i];
		while (cur != NULL)
		{
			printf("Index: %d\tKey: %s\tVal: %#x\tNext: %#x\n", i, cur->key, (int)cur->val, (int)cur->next);
			HashNode_T* next = cur->next;	
			free( cur->key );
			free( cur->val );
			free( cur );
			cur = next;
		}
	}
}

BOOL Hash_Put(HashTable_T* table, String key, void* val)
{
	U32 index = table->hash_func( key ) % table->size;
	HashNode_T* cur = table->table[index];
	HashNode_T* last = cur; 

	while (cur != NULL)
	{
		if ( !strcmp( key, cur->key ) )
		{
			cur->val = val;
			break;
		}
		last = cur;
		cur = cur->next;
	}

	if (cur == NULL)
	{
		HashNode_T* node = (HashNode_T*) malloc( sizeof(HashNode_T) );
		node->key = (String) strdup( key );
		node->val = val;
		node->next = NULL;	

		if (last != NULL)
		{
			last->next = node;
		}
		else
		{
			table->table[ index ] = node;
		}
	}
	return TRUE;
}

void* Hash_Get(HashTable_T* table, String key)
{
	void* ret = NULL;
	U32 index= table->hash_func( key ) % table->size;
	HashNode_T* node = table->table[ index ];
	while ( node != NULL )
	{
		if ( !strcmp( key, node->key ) )
		{
			ret = node->val;
			break;
		}
		node = node->next;
	}
	return ret;
}

U32 Hash_Delete(HashTable_T* table, String key)
{
	return 0;
}

U32 Hash_Resize(HashTable_T* table, U32 size)
{
	return 0;
}

