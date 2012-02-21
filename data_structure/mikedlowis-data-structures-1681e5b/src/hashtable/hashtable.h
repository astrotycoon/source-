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
#ifndef HASHTABLE_H
#define HASHTABLE_H 

#include "common.h"

typedef U32 (*HashFuncPtr_T) (String) ;
typedef struct HashNode
{
	String key;
	void* val;
	struct HashNode* next;
} HashNode_T;

typedef struct
{
	U32 size;
	HashNode_T** table;
	HashFuncPtr_T hash_func;
} HashTable_T;

HashTable_T* Hash_New(U32 size, HashFuncPtr_T fn);
void Hash_Free(HashTable_T* table);
BOOL Hash_Put(HashTable_T* table, String key, void* val);
void* Hash_Get(HashTable_T* table, String key);
U32 Hash_Delete(HashTable_T* table, String key);
U32 Hash_Resize(HashTable_T* table, U32 size);

#endif
