/*
	This file is part of jmlist.

	jmlist is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	jmlist is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with jmlist.  If not, see <http://www.gnu.org/licenses/>.

	Copyright (C) 2009 Jean-François Mousinho (jean.mousinho@ist.utl.pt)
*/

/**
  * funções a implementar:
	- jmlist_initialize(flags..,)
		- com lista interna de jmlists ou nao
		- com ou sem tags
	- jmlist_lookup(
		- procurar entrada numa lista, comparacao via *jmlist
	- jmlist_free
	- jmlist_flag_test
	- jmlist_flag_set

  dependendo do tipo de lista, existem várias funções para criar,
  allocar, remover, substituir, cortar, inserir, etc. ou seja
  para cada tipo de lista existe um conjunto de funções para gerir
  a mesma. No futuro estas funções deverão poder ter callbacks
  do cliente.

  v1.0.1:
	- allow inteligent gradient for indexed lists. this grandient
	should be based on past information.
 
  v1.0-r5:
	- indexed list OK (with params.malloc_inc)
	- linked list OK

 useful links:
  http://code.google.com/p/google-sparsehash/ 

  indexed lists dont have an usage bit map, this is required to
  work with non-SHIFTED idx lists, it isn't good to associate
  ptr=0 as the empty entry. The entry ptr should be able to be
  any value...
*/

#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>

#include "jmlist.h"

/* private functions declarations */
jmlist_status ijmlist_idx_get_by_index(jmlist jml,jmlist_index index,void **ptr);
jmlist_status ijmlist_idx_insert(jmlist jml,void *ptr);
jmlist_status ijmlist_idx_remove_by_ptr(jmlist jml,void *ptr);
jmlist_status ijmlist_idx_dump(jmlist jml);
jmlist_status ijmlist_idx_pop(jmlist jml,void **ptr);
jmlist_status ijmlist_idx_push(jmlist jml,void *ptr);
jmlist_status ijmlist_idx_free(jmlist jml); 
jmlist_status ijmlist_idx_set_capacity(jmlist jml,jmlist_index capacity);
jmlist_status ijmlist_idx_ptr_exists(jmlist jml,void *ptr,jmlist_lookup_result *result);
jmlist_status ijmlist_idx_is_fragmented(jmlist jml,bool force_seeker,bool *fragmented);
jmlist_status ijmlist_idx_remove_by_index(jmlist jml,jmlist_index index);
jmlist_status ijmlist_idx_replace_by_index(jmlist jml,jmlist_index index,void *new_ptr);
jmlist_status ijmlist_idx_seek_start(jmlist jml,jmlist_seek_handle *handle_ptr);
jmlist_status ijmlist_idx_seek_next(jmlist jml,jmlist_seek_handle *handle_ptr,void **ptr);

/* linked list routines */
jmlist_status ijmlist_lnk_get_by_index(jmlist jml,jmlist_index index,void **ptr);
jmlist_status ijmlist_lnk_insert(jmlist jml,void *ptr);
jmlist_status ijmlist_lnk_remove_by_ptr(jmlist jml,void *ptr);
jmlist_status ijmlist_lnk_dump(jmlist jml);
jmlist_status ijmlist_lnk_pop(jmlist jml,void **ptr);
jmlist_status ijmlist_lnk_push(jmlist jml,void *ptr);
jmlist_status ijmlist_lnk_free(jmlist jml);
jmlist_status ijmlist_lnk_ptr_exists(jmlist jml,void *ptr,jmlist_lookup_result *result);
jmlist_status ijmlist_lnk_remove_by_index(jmlist jml,jmlist_index index);
jmlist_status ijmlist_lnk_replace_by_index(jmlist jml,jmlist_index index,void *new_ptr);
jmlist_status ijmlist_lnk_seek_start(jmlist jml,jmlist_seek_handle *handle_ptr);
jmlist_status ijmlist_lnk_seek_next(jmlist jml,jmlist_seek_handle *handle_ptr,void **ptr);

/* associative list routines */
#ifdef WITH_ASSOC_LIST
jmlist_status ijmlist_ass_get_by_index(jmlist jml,jmlist_index index,void **ptr);
jmlist_status ijmlist_ass_get_by_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len,void **ptr);
jmlist_status ijmlist_ass_insert(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len,void *ptr);
jmlist_status ijmlist_ass_remove_by_ptr(jmlist jml,void *ptr);
jmlist_status ijmlist_ass_remove_by_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len);
jmlist_status ijmlist_ass_dump(jmlist jml);
jmlist_status ijmlist_ass_pop(jmlist jml,void **ptr);
jmlist_status ijmlist_ass_push(jmlist jml,void *ptr);
jmlist_status ijmlist_ass_free(jmlist jml);
jmlist_status ijmlist_ass_ptr_exists(jmlist jml,void *ptr,jmlist_lookup_result *result);
jmlist_status ijmlist_ass_remove_by_index(jmlist jml,jmlist_index index);
jmlist_status ijmlist_ass_replace_by_index(jmlist jml,jmlist_index index,void *new_ptr);
jmlist_status ijmlist_ass_seek_start(jmlist jml,jmlist_seek_handle *handle_ptr);
jmlist_status ijmlist_ass_seek_next(jmlist jml,jmlist_seek_handle *handle_ptr,void **ptr);
#endif

unsigned int jmlist_malloc_counter = 0;
unsigned int jmlist_free_counter = 0;
jmlist jmlist_ilist = 0;
struct _jmlist_init_params jmlist_cfg;
FILE *fnull = 0;
jmlist_status jmlist_errno = JMLIST_ERROR_SUCCESS;
struct _jmlist_memory_info jmlist_mem;

/*
   jmlist_initialize

   initialize jmlist library, in params argument the client
   passes information about the output channels (for error, debugging
   and verbose messages) and some flags (check jmlist.h for more
   information about the flags.
*/
jmlist_status
jmlist_initialize(jmlist_init_params params)
{
	/* clear configuration */
	memset(&jmlist_cfg,0,sizeof(jmlist_cfg));
	memset(&jmlist_mem,0,sizeof(struct _jmlist_memory_info));
	
	if( params->flags & JMLIST_FLAG_DEBUG )
		jmlist_cfg.flags |= JMLIST_FLAG_DEBUG;
	
	if( params->flags & JMLIST_FLAG_VERBOSE )
		jmlist_cfg.flags |= JMLIST_FLAG_VERBOSE;
	
	jmlist_cfg.flags = params->flags;
	
	/* if any file descriptor is zero, use /dev/null */
	fnull = fopen("/dev/null","w");
	jmlist_cfg.fverbose = params->fverbose ? params->fverbose : fnull;
	jmlist_cfg.fdebug = params->fdebug ? params->fdebug : fnull;
	jmlist_cfg.fdump = params->fdump ? params->fdump : stdout;

	/* if internal list is active, create the internal list */
	if( params->flags & JMLIST_FLAG_INTERNAL_LIST )
	{
		struct _jmlist_params tparams;
		tparams.flags = JMLIST_LINKED | JMLIST_LNK_INTERNAL;
		if( jmlist_create(&jmlist_ilist,&tparams) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;
		
		jmlist_cfg.flags |= JMLIST_FLAG_INTERNAL_LIST;
	}	

	return JMLIST_ERROR_SUCCESS;
}

/*
   jmlist_uninitialize

   free allocated lists if internal list flag is active, also free the
   fnull file descriptor if it was used.
*/
jmlist_status
jmlist_uninitialize(void)
{
	/* test for internal list flag, warn if there's any list to be freed */
	if( jmlist_cfg.flags & JMLIST_FLAG_INTERNAL_LIST )
	{
		if( jmlist_ilist->lnk_list.usage )
			jmlist_free_all();
		
		/* free linked list */
		jmlist_free(jmlist_ilist);
		jmlist_ilist = 0;
	}

	/* close fnull if was opened */
	if( fnull )
	{
		fclose(fnull);
		fnull = 0;
	}
	
	return JMLIST_ERROR_SUCCESS;
}

/*
 * set new indexed list capacity, this function can only be used to make the new list grow,
 * or to reduce size only if there's empty entries in the tail (*).
 *
 * (*) requires JMLIST_IDX_USE_SHIFT flag to be set.
 */
jmlist_status
ijmlist_idx_set_capacity(jmlist jml,jmlist_index capacity)
{
	jmlist_debug(__func__,"called with capacity = %u",capacity);
	
	if( !capacity )
	{
		/* trying to clear list? */
		if( jml->idx_list.usage )
		{
			jmlist_debug(__func__,"cannot change list capacity to 0 when its not empty!");
			jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
			return JMLIST_ERROR_FAILURE;
		}
		
		jmlist_debug(__func__,"freeing list buffer (plist=%p, capacity=%u)",jml->idx_list.plist,jml->idx_list.capacity);
		
		if( ijmlist_idx_free(jml) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;
		
		jmlist_debug(__func__,"clearing plist, capacity and usage");
		
		jmlist_mem.idx_list.total -= jml->idx_list.capacity*sizeof(void*);
		jmlist_mem.idx_list.used -= jml->idx_list.usage*sizeof(void*);
		jmlist_debug(__func__,"new jml_mem.idx_list.total is %u",jmlist_mem.idx_list.total);
		jmlist_debug(__func__,"new jml_mem.idx_list.used is %u",jmlist_mem.idx_list.used);
		
		jml->idx_list.plist = 0;
		jml->idx_list.usage = jml->idx_list.capacity;
		jml->idx_list.capacity = 0;
		
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	if( capacity < jml->idx_list.capacity )
	{
		jmlist_debug(__func__,"cannot shrink list (current capacity is %u, asked new capacity is %u)",
					 jml->idx_list.capacity,capacity);
		
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_UNSUPPORTED;
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"reallocating plist (old plist is %p) from capacity %u to capacity %u",
					jml->idx_list.plist, jml->idx_list.capacity, capacity);
		
	jml->idx_list.plist = realloc(jml->idx_list.plist,sizeof(void*)*capacity);
	if( !jml->idx_list.plist )
	{
		jmlist_debug(__func__,"realloc failed on plist=%p and size=%u!",
					 jml->idx_list.plist,capacity);
	
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_MALLOC;
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"reallocation success, new plist is %p",jml->idx_list.plist);
	
	/* clear new entries and update list parameters */
	jmlist_debug(__func__,"clearing new list entries, from %u (%u entries) from address %p",
				 jml->idx_list.capacity,(capacity-jml->idx_list.capacity),
				 jml->idx_list.plist+jml->idx_list.capacity);
	
	jmlist_mem.idx_list.total += (capacity*sizeof(void*) - jml->idx_list.capacity*sizeof(void*));
	jmlist_mem.idx_list.used -= jml->idx_list.usage*sizeof(void*);
	jmlist_debug(__func__,"new jml_mem.idx_list.total is %u",jmlist_mem.idx_list.total);
	jmlist_debug(__func__,"new jml_mem.idx_list.used is %u",jmlist_mem.idx_list.used);

	jmlist_debug(__func__,"memset %u from %p of %u bytes long",JMLIST_EMPTY_PTR,
		jml->idx_list.plist+jml->idx_list.capacity,sizeof(void*)*(capacity-jml->idx_list.capacity));

	void **ptr_seeker = jml->idx_list.plist+jml->idx_list.capacity;
	for( jmlist_index i = 0 ; i < (capacity-jml->idx_list.capacity) ; i++,ptr_seeker++ ) {
			*ptr_seeker = JMLIST_EMPTY_PTR;
	}
	jml->idx_list.capacity = capacity;
	jml->idx_list.usage = 0;
		
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
 * free indexed list, there's only one block of memory used in indexed lists,
 * the pointer to this block is saved in jml->idx_list.plist after that,
 * cleanup information about the list (capacity=0, usage=0).
 */
jmlist_status
ijmlist_idx_free(jmlist jml)
{
	jmlist_debug(__func__,"called with jml=%p",jml);
	
	if( jml->idx_list.plist )
		free(jml->idx_list.plist);
	
	jmlist_mem.idx_list.total -= jml->idx_list.capacity*sizeof(void*);
	jmlist_mem.idx_list.used -= jml->idx_list.usage*sizeof(void*);
	jmlist_debug(__func__,"new jml_mem.idx_list.total is %u",jmlist_mem.idx_list.total);
	jmlist_debug(__func__,"new jml_mem.idx_list.used is %u",jmlist_mem.idx_list.used);

	jml->idx_list.plist = 0;
	jml->idx_list.capacity = 0;
	jml->idx_list.usage = 0;
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

jmlist_status
ijmlist_lnk_free(jmlist jml)
{
	jmlist_debug(__func__,"called with jml=%p",jml);
	
	linked_entry *pseeker = jml->lnk_list.phead;
	linked_entry *pnext;
	while( pseeker )
	{
		pnext = pseeker->next;
		jmlist_debug(__func__,"freeing linked entry with p=%p, next is p=%p",pseeker,pnext);
		free(pseeker);
		
		jmlist_mem.lnk_list.total -= sizeof(struct _linked_entry);
		jmlist_mem.lnk_list.used -= sizeof(struct _linked_entry);
		jmlist_debug(__func__,"new jml_mem.lnk_list.total is %u",jmlist_mem.lnk_list.total);
		jmlist_debug(__func__,"new jml_mem.lnk_list.used is %u",jmlist_mem.lnk_list.used);
		pseeker = pnext;
	}
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   ijmlist_idx_push

   push an item on the list head, this requires shifting all the items one place
   above so there's one free place in the head of the list. there are better ways
   of handling this, one is using an extra variable in idx_list that saves the
   last used index.
 
   removing an entry from the middle of the list, in which the list doesn't have
   the flag JMLIST_IDX_USE_SHIFT will create an "hole" in the list. pushing/poping
   from a fragmented list will also shift these empty entries so, it should be
   forbidden to push/pop from a non-JMLIST_IDX_USE_SHIFT since these lists might
   be fragmented.
 
   push operations on indexed lists are O(N) worst case.
 */
jmlist_status
ijmlist_idx_push(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p and ptr=%p",jml,ptr);

	/* if the list is full or zero capacity, we need to increment its size */
	if( !jml->idx_list.capacity || (jml->idx_list.capacity == jml->idx_list.usage) )
	{
		jmlist_debug(__func__,"extending list size to %u",jml->idx_list.capacity + jml->idx_list.malloc_inc);
		if( ijmlist_idx_set_capacity(jml,jml->idx_list.capacity + jml->idx_list.malloc_inc) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;
	}
	
	/* starting in the end of the list, shift items one position up */
	jmlist_debug(__func__,"shifting entries one position");
	
	jmlist_index i;
	for( i = jml->idx_list.usage ; i >= 1 ; i-- )
		jml->idx_list.plist[i] = jml->idx_list.plist[i-1];
	
	jmlist_debug(__func__,"pushing ptr=%p into the head of the list.",ptr);
	jml->idx_list.plist[0] = ptr;
	jml->idx_list.usage++;
	
	jmlist_mem.idx_list.used += sizeof(void*);
	jmlist_debug(__func__,"new jml_mem.idx_list.used is %u",jmlist_mem.idx_list.used);
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   ijmlist_lnk_push

   Push routine for linked lists, complexity is O(k). Each time an entry
   is pushed into the list, a new structure is allocated of size = linked_entry.
   The pointer value is copied into this structure.
 */
jmlist_status
ijmlist_lnk_push(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p",jml,ptr);

	jmlist_debug(__func__,"allocating linked entry");
	linked_entry *pentry = (linked_entry*)malloc(sizeof(linked_entry));
	if( !pentry ) 
	{
		jmlist_debug(__func__,"malloc failed (size %u)!",sizeof(linked_entry));
		jmlist_errno = JMLIST_ERROR_MALLOC;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	jmlist_debug(__func__,"new linked entry pentry=%p allocated",pentry);
	
	jmlist_mem.lnk_list.total += sizeof(struct _linked_entry);
	jmlist_mem.lnk_list.used += sizeof(struct _linked_entry);
	jmlist_debug(__func__,"new jml_mem.lnk_list.total is %u",jmlist_mem.lnk_list.total);
	jmlist_debug(__func__,"new jml_mem.lnk_list.used is %u",jmlist_mem.lnk_list.used);

	/* initialize entry */
	pentry->ptr = ptr;
	pentry->next = jml->lnk_list.phead;
	jmlist_debug(__func__,"initialized new entry successfuly (ptr=%p, next=%p)",pentry->ptr,pentry->next);
	
	jmlist_debug(__func__,"inserting pentry in head of list (current head phead=%p, usage=%u)",
				 jml->lnk_list.phead,jml->lnk_list.usage);
	
	jml->lnk_list.phead = pentry;
	jml->lnk_list.usage++;
	
	jmlist_debug(__func__,"now head is phead=%p with phead->next=%p (usage=%u)",
				 jml->lnk_list.phead,pentry->next,jml->lnk_list.usage);
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
 * pop operations on indexed lists are O(N) worst case
 */
jmlist_status
ijmlist_idx_pop(jmlist jml,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p and ptr=%p",jml,ptr);
	
	/* shouldn't use push/pop on a fragmented idx list */
	if( !(jml->flags & JMLIST_IDX_USE_SHIFT) )
	{
		/* check for fragmentation */
		bool fragmented = false;
		if( jmlist_is_fragmented(jml,false,&fragmented) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;
		
		if( fragmented == true )
		{
			jmlist_debug(__func__,"unable to process pop on idx list in a fragmented list (try use JMLIST_IDX_USE_SHIFT flag on this list)");
			jmlist_errno = JMLIST_ERROR_FAILURE;
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
	}
	
	/* by using JMLIST_IDX_USE_SHIFT flag, it should be quaranted that the list
	 doesn't fragment. */
	
	if( !jml->idx_list.capacity || !jml->idx_list.usage )
	{
		jmlist_debug(__func__,"cannot pop an empty list's entry!");
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !ptr )
	{
		jmlist_debug(__func__,"invalid ptr specified! (ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* lets pop out one entry... this requires a shift on the list after. */
	jmlist_index i;
	*ptr = jml->idx_list.plist[0];
	jmlist_debug(__func__,"poped entry has ptr=%p, shifting the list entries.",*ptr);
	for ( i = 0 ; i < (jml->idx_list.capacity-1) ; i++) {
		jml->idx_list.plist[i] = jml->idx_list.plist[i+1];
	}
	/* clear last entry */
	jml->idx_list.plist[i] = JMLIST_EMPTY_PTR;
	jml->idx_list.usage--;
	
	jmlist_mem.idx_list.used -= sizeof(void*);
	jmlist_debug(__func__,"new jml_mem.idx_list.used is %u",jmlist_mem.idx_list.used);
	
	/* done.. */
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
 * pop operations on linked lists are O(1)
 */
jmlist_status
ijmlist_lnk_pop(jmlist jml,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p and ptr=%p",jml,ptr);

	if( !ptr )
	{
		jmlist_debug(__func__,"invalid ptr pointer specified (ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	/* trying to pop from empty list? */
	if( !jml->lnk_list.usage )
	{
		jmlist_debug(__func__,"trying to pop from empty list");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}
	
	DCHECKSTART
	if( !jml->lnk_list.phead )
	{
		/* damaged linked list.. */
		jmlist_debug(__func__,"damaged linked list, usage!=0 but phead=0");
		jmlist_errno = JMLIST_ERROR_DAMAGED_LIST;
		return JMLIST_ERROR_FAILURE;
	}
	DCHECKEND
	
	/* pop out the entry, return the ptr and free the entry */
	linked_entry *pentry;
	pentry = jml->lnk_list.phead;
	
	jmlist_debug(__func__,"poped entry from list (pentry=%p)",pentry);
	jml->lnk_list.phead = pentry->next;
	
	jmlist_debug(__func__,"new list head is phead=%p",pentry->next);
	*ptr = pentry->ptr;
	
	jmlist_debug(__func__,"freeing pentry=%p",pentry);
	free(pentry);
	
	jml->lnk_list.usage--;
	jmlist_debug(__func__,"decreased usage to %u",jml->lnk_list.usage);
	
	jmlist_mem.lnk_list.total -= sizeof(struct _linked_entry);
	jmlist_mem.lnk_list.used -= sizeof(struct _linked_entry);
	jmlist_debug(__func__,"new jml_mem.lnk_list.total is %u",jmlist_mem.lnk_list.total);
	jmlist_debug(__func__,"new jml_mem.lnk_list.used is %u",jmlist_mem.lnk_list.used);
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

jmlist_status
ijmlist_idx_dump(jmlist jml)
{
	/* dump list information */
	fprintf(jmlist_cfg.fdump,"capacity: %u, usage: %u\n",jml->idx_list.capacity,jml->idx_list.usage);

	/* if we've any entries in the list, plot their index-value */
	if( jml->idx_list.usage )
	{
		fprintf(jmlist_cfg.fdump,"list of entries: \n");
		jmlist_index i;
		for( i = 0 ; i < jml->idx_list.capacity ; i++ )
			fprintf(jmlist_cfg.fdump,"%u %p\n",i,jml->idx_list.plist[i]);
	}
	
	return JMLIST_ERROR_SUCCESS;
}

jmlist_status
ijmlist_lnk_dump(jmlist jml)
{
	jmlist_debug(__func__,"called with jml=%p",jml);
	
	fprintf(jmlist_cfg.fdump,"linked list jml=%p has usage=%u and phead=%p\n",(void*)jml,jml->lnk_list.usage,(void*)jml->lnk_list.phead);
	
	linked_entry *pseeker = jml->lnk_list.phead;
	
	if( pseeker )
		fprintf(jmlist_cfg.fdump,"list of entries:\n");
	
	jmlist_index index = 0;
	while( pseeker )
	{
		fprintf(jmlist_cfg.fdump,"  %08X: pentry=%p pentry->next=%p ptr=%p\n",index,(void*)pseeker,(void*)pseeker->next,pseeker->ptr);
		pseeker = pseeker->next;
		index++;
	}
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
 * remove operations on indexed lists are O(N) if shift is ON, O(1) otherwise.
 */
jmlist_status
ijmlist_idx_remove_by_ptr(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p and ptr=%p",jml,ptr);
	
	/* check if the list is empty */
	if( jml->idx_list.usage == 0 )
	{
		jmlist_debug(__func__,"cannot remove entries from an empty list");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* entry found, lets remove it.. */
	jmlist_index i;
	for( i = 0 ; i < jml->idx_list.capacity ; i++ )
	{
		if( jml->idx_list.plist[i] != ptr )
			continue;
		
		jmlist_debug(__func__,"found entry (ptr=%p) from list %p in index %u",ptr,jml,i);
		
		/* found the entry, clear it */
		jml->idx_list.plist[i] = JMLIST_EMPTY_PTR;
		jml->idx_list.usage--;
		
		jmlist_mem.idx_list.used -= sizeof(void*);
		jmlist_debug(__func__,"new jml_mem.idx_list.used is %u",jmlist_mem.idx_list.used);
		
		jmlist_debug(__func__,"entry ptr=%p removed from list %p, new usage is %u",ptr,jml,jml->idx_list.usage);

		/* if shift is activated, remove and shift, otherwise just remove */
		if( jml->flags & JMLIST_IDX_USE_SHIFT )
		{
			jmlist_debug(__func__,"JMLIST_IDX_USE_SHIFT activated, shifting list entries.");
			
			for( ; i < (jml->idx_list.capacity - 1) ; i++ )
				jml->idx_list.plist[i] = jml->idx_list.plist[i+1];
		}
		
		if( (jml->flags & JMLIST_IDX_USE_FRAG_FLAG) && (jml->idx_list.fragmented == false) )
		{
			if( (i < (jml->idx_list.capacity-1)) && (jml->idx_list.plist[i+1] != JMLIST_EMPTY_PTR) )
				jml->idx_list.fragmented = true;
			
			/* else we'll keep it in false (not fragmented) */
		}
		
		return JMLIST_ERROR_SUCCESS;
	}
	
	/* entry not found, this should be detected earlier tho... */
	jmlist_debug(__func__,"couldn't found entry (ptr=%p) in the list!",ptr);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}

/*
 * remove operations on linked lists are O(N) worst case
 */
jmlist_status
ijmlist_lnk_remove_by_ptr(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p",jml,ptr);

	/* trying to pop from empty list? */
	if( !jml->lnk_list.usage )
	{
		jmlist_debug(__func__,"trying to remove from empty list");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"seeking ptr=%p in the linked list",ptr);
	
	linked_entry *pseeker = jml->lnk_list.phead;
	linked_entry *pprevious = pseeker;
	while( pseeker )
	{
		if( pseeker->ptr != ptr )
		{
			pprevious = pseeker;
			pseeker = pseeker->next;
			continue;
		}
		
		jmlist_debug(__func__,"found entry, pseeker=%p pseeker->next=%p",pseeker,pseeker->next);
		
		if( pprevious == pseeker )
		{
			/* we're at HEAD */
			jmlist_debug(__func__,"entry is from head, lets pop it instead...");
			void *junk;
			return ijmlist_lnk_pop(jml,&junk);
		}
		
		jmlist_debug(__func__,"unlinking entry");
		pprevious->next = pseeker->next;
		jml->lnk_list.usage--;
		
		jmlist_mem.lnk_list.total -= sizeof(struct _linked_entry);
		jmlist_mem.lnk_list.used -= sizeof(struct _linked_entry);
		jmlist_debug(__func__,"new jml_mem.lnk_list.total is %u",jmlist_mem.lnk_list.total);
		jmlist_debug(__func__,"new jml_mem.lnk_list.used is %u",jmlist_mem.lnk_list.used);
		
		
		jmlist_debug(__func__,"unlinked successfuly, new usage is %u, freeing entry",jml->lnk_list.usage);
		free(pseeker);
		
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	jmlist_debug(__func__,"couldn't find entry with ptr=%p in list jml=%p",ptr,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}

/*
 * insert new entry in the list, grow it if necessary.
 */
jmlist_status
ijmlist_idx_insert(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p and ptr=%p",jml,ptr);
	
	jmlist_debug(__func__,"list (jml=%p), capacity %u, usage %u",jml,jml->idx_list.capacity,jml->idx_list.usage);
	/* check if list is empty or full */
	if( (jml->idx_list.capacity == 0) || (jml->idx_list.capacity == jml->idx_list.usage) )
	{
		jmlist_debug(__func__,"growing the list from capacity %u to %u",
					 jml->idx_list.capacity, jml->idx_list.capacity + jml->idx_list.malloc_inc);
		
		if( ijmlist_idx_set_capacity(jml, jml->idx_list.capacity + jml->idx_list.malloc_inc) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"looking for free entry in list");
	
	/* lookup an empty entry and insert there */
	jmlist_index i = 0;
	while( jml->idx_list.plist[i] != JMLIST_EMPTY_PTR ) i++;
	jmlist_debug(__func__,"found free entry in %u position (plist[%u]=%p)",i,i,jml->idx_list.plist[i]);
	jml->idx_list.plist[i] = ptr;
	jml->idx_list.usage++;
	
	jmlist_mem.idx_list.used += sizeof(void*);
	jmlist_debug(__func__,"new jml_mem.idx_list.used is %u",jmlist_mem.idx_list.used);
	
	jmlist_debug(__func__,"inserted ptr=%p in %u position, usage changed to %u",ptr,i,jml->idx_list.usage);
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

jmlist_status
ijmlist_lnk_insert(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p and ptr=%p",jml,ptr);
	
	if( jml->flags & JMLIST_LNK_INSERT_AT_TAIL )
	{
		jmlist_debug(__func__,"inserting at tail");
		
		linked_entry *pseeker = jml->lnk_list.phead;
		while( pseeker->next ) pseeker = pseeker->next;
		
		/* found entry */
		jmlist_debug(__func__,"found tail at pseeker=%p",pseeker);

		jmlist_debug(__func__,"allocating linked entry");
		linked_entry *pentry = (linked_entry*)malloc(sizeof(linked_entry));
		if( !pentry ) 
		{
			jmlist_debug(__func__,"malloc failed (size %u)!",sizeof(linked_entry));
			jmlist_errno = JMLIST_ERROR_MALLOC;
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
		jmlist_debug(__func__,"new linked entry pentry=%p allocated",pentry);
		
		/* initialize entry */
		pentry->ptr = ptr;
		pentry->next = 0;
		jmlist_debug(__func__,"initialized new entry successfuly (ptr=%p, next=%p)",pentry->ptr,pentry->next);
		
		pseeker->next = pentry;
		
		jml->lnk_list.usage++;
		jmlist_debug(__func__,"new usage of list is %u",jml->lnk_list.usage);
		
		jmlist_mem.lnk_list.total += sizeof(struct _linked_entry);
		jmlist_mem.lnk_list.used += sizeof(struct _linked_entry);
		jmlist_debug(__func__,"new jml_mem.lnk_list.total is %u",jmlist_mem.lnk_list.total);
		jmlist_debug(__func__,"new jml_mem.lnk_list.used is %u",jmlist_mem.lnk_list.used);
		
	} else
	{
		jmlist_debug(__func__,"inserting at head is same as push, changing to push routine.");
		return ijmlist_lnk_push(jml,ptr);
	}
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
 * create a jmlist, can have one of various types and flags.
 * for now linked lists and indexed lists are supported.
 * if global flags include internal list, an entry should be
 * added to this internal list.
 */
jmlist_status
jmlist_create(jmlist *new_jml,jmlist_params params)
{
	jmlist_debug(__func__,"called with new_jml=%p and params=%p",new_jml,params);
	
	/* verify new_jml argument */
	if( !new_jml )
	{
		jmlist_debug(__func__,"invalid params pointer specified (params=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"allocating a jmlist structure");
	
	/* allocate a new jmlist structure */
	*new_jml = (jmlist)malloc(sizeof(struct _jmlist));
	
	if( !new_jml )
	{
		jmlist_debug(__func__,"malloc failed (size %u)!",sizeof(struct _jmlist));
		jmlist_errno = JMLIST_ERROR_MALLOC;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_mem.total += sizeof(struct _jmlist);
	jmlist_mem.used += sizeof(struct _jmlist);
	jmlist_debug(__func__,"new jml_mem.total is %u",jmlist_mem.total);
	jmlist_debug(__func__,"new jml_mem.used is %u",jmlist_mem.used);
	
	jmlist_debug(__func__,"initializing new jmlist (new_jml=%p)",new_jml);
	
	memset(*new_jml,0,sizeof(struct _jmlist));
	(*new_jml)->flags = params->flags;
	(*new_jml)->seeking = false;
	memcpy((*new_jml)->tag,params->tag,sizeof((*new_jml)->tag));
	
	jmlist_debug(__func__,"initialized new jmlist successfuly (new_jml=%p)",new_jml);
	
	/* add list to internal list if internal list flag is set */
	if( (jmlist_cfg.flags & JMLIST_FLAG_INTERNAL_LIST) && !(params->flags & JMLIST_LNK_INTERNAL) )
	{
		jmlist_debug(__func__,"adding new list (jml=%p) to internal list",*new_jml);
		jmlist_push(jmlist_ilist,*new_jml);
	}
	
	jmlist_debug(__func__,"applying list type initialization (new_jml=%p)",new_jml);
	
	if( params->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"indexed list flag detected (new_jml=%p)",new_jml);
		
		/* indexed list, initialize indexed list parameters */
		(*new_jml)->idx_list.malloc_inc = params->idx_list.malloc_inc ?
			params->idx_list.malloc_inc : JMLIST_IDXLIST_DEF_MALLOC_INC;
		
		jmlist_debug(__func__,"malloc_inc set to %u (new_jml=%p)",(*new_jml)->idx_list.malloc_inc,*new_jml);
	} else if( params->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"linked list flag detected (new_jml=%p)",*new_jml);
		
		/* linked-list type, initialize list parameters */
	} else if( params->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"associative list flag detected (new_jml=%p)",*new_jml);

		/* associative-list type, initialize list parameters */
	} else
	{
		jmlist_debug(__func__,"invalid or unsupported list type new_jml=%p, flags=%u",*new_jml,(*new_jml)->flags);
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		
		jmlist_debug(__func__,"freeing new_jml structure memory (new_jml=%p)",*new_jml);
		free(new_jml);
		
		jmlist_mem.total -= sizeof(struct _jmlist);
		jmlist_mem.used -= sizeof(struct _jmlist);
		jmlist_debug(__func__,"new jml_mem.total is %u",jmlist_mem.total);
		jmlist_debug(__func__,"new jml_mem.used is %u",jmlist_mem.used);
		
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   jmlist_ptr_exists

   lookup if a specific ptr value exists on the list.
   result is returned in result argument and can be jmlist_entry_found
   or jmlist_entry_not_found.
*/
jmlist_status
jmlist_ptr_exists(jmlist jml,void *ptr,jmlist_lookup_result *result)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p, result=%p",jml,ptr,result);

	if( !result )
	{
		jmlist_debug(__func__,"invalid result pointer specified (result=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* depending on type of list, call its internal lookup function */
	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list lookup routine");
		return ijmlist_idx_ptr_exists(jml,ptr,result);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list lookup routine");
		return ijmlist_lnk_ptr_exists(jml,ptr,result);
	}
	
	jmlist_debug(__func__,"invalid or unsuported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

/*
 * indexed operations on indexed lists are O(1)
 */
jmlist_status
ijmlist_idx_get_by_index(jmlist jml,jmlist_index index,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u, ptr=%p",jml,index,ptr);

	if( !ptr )
	{
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	if( !jml->idx_list.usage )
	{
		jmlist_debug(__func__,"cannot get by index on empty list (lnk_list.usage=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}

	/* shouldn't use get on a fragmented idx list */
	if( !(jml->flags & JMLIST_IDX_USE_SHIFT) )
	{
		/* check for fragmentation */
		bool fragmented = false;
		if( jmlist_is_fragmented(jml,false,&fragmented) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;
		
		if( fragmented == true )
		{
			jmlist_debug(__func__,"cannot use this function with possible fragmented indexed lists, use of JMLIST_IDX_USE_SHIFT is required");
			jmlist_errno = JMLIST_ERROR_FAILURE;
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
	}
	
	/* check bounds */
	if( index >= jml->idx_list.usage )
	{
		jmlist_debug(__func__,"index %u is out of bounds with list jml=%p which has %u entries",index,jml,jml->idx_list.usage);
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* finally return item.. */
	jmlist_debug(__func__,"index %u has ptr=%p associated",index,jml->idx_list.plist[index]);
	*ptr = jml->idx_list.plist[index];
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
 * indexed operations on linked lists are O(N) (worst case).
 */
jmlist_status
ijmlist_lnk_get_by_index(jmlist jml,jmlist_index index,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);

	if( !ptr )
	{
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	if( !jml->lnk_list.usage )
	{
		jmlist_debug(__func__,"cannot get by index on empty list (lnk_list.usage=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}
	
	/* check bounds! */
	if( index >= jml->lnk_list.usage )
	{
		jmlist_debug(__func__,"index %u is out of bounds with list jml=%p which has %u entries",index,jml,jml->lnk_list.usage);
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"parsing linked list for indexed item %u",index);
	
	jmlist_index i = 0;
	linked_entry *pseeker = jml->lnk_list.phead;
	/* Fix: when pseeker = 0, trow error! */
	while( i != index )
	{
		pseeker = pseeker->next;
		i++;
	}

	*ptr = pseeker->ptr;
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   jmlist_get_by_index

   a way of getting an entry from the list is by using an index,
   this is the public function to obtain the entry ptr using the index.
   depeding on the list type it will call a specific function for
   each type.
*/
jmlist_status
jmlist_get_by_index(jmlist jml,jmlist_index index,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);
	
	if( !ptr )
	{
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list get_by_index routine.");
		return ijmlist_idx_get_by_index(jml,index,ptr);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list get_by_index routine.");
		return ijmlist_lnk_get_by_index(jml,index,ptr);
	}
#ifdef WITH_ASSOC_LIST
	else if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"passing control to the associative list get_by_index routine.");
		return ijmlist_ass_get_by_index(jml,index,ptr);
	}
#endif
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

/*
   jmlist_remove_by_ptr
  
   remove an entry from the list using the ptr, remove is different from free,
   remove will unlink the entry from the list and after that, the entry is freed.
   free should not be called to remove an entry from the list correctly. the right
   way is to call jmlist_remove_* function. jmlist_free should only be used directly
   when the entry was not yet linked to the list.
*/
jmlist_status
jmlist_remove_by_ptr(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p",jml,ptr);
	
	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list remove_by_ptr routine.");
		return ijmlist_idx_remove_by_ptr(jml,ptr);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list remove_by_ptr routine.");
		return ijmlist_lnk_remove_by_ptr(jml,ptr);
	}
#ifdef WITH_ASSOC_LIST
	else if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"passing control to the associative list remove_by_ptr routine.");
		return ijmlist_ass_remove_by_ptr(jml,ptr);
	}
#endif
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

/*
   jmlist_free

   frees the whole list, if it has entries all of them will also be freed.
   if internal list is being used it will also be updated, this list pointer
   will be removed from that list.
*/
jmlist_status
jmlist_free(jmlist jml)
{
	jmlist_debug(__func__,"called with jml=%p");
	
	/* lookup in internal list if there's one.. */
	if( (jml != jmlist_ilist) && (jmlist_cfg.flags & JMLIST_FLAG_INTERNAL_LIST) )
	{
		jmlist_debug(__func__,"removing jml=%p from the internal list",jml);
	
		/* found the entry in the internal list, remove it */
		if( jmlist_remove_by_ptr(jmlist_ilist,jml) == JMLIST_ERROR_FAILURE )
		{
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
	}

	/* free the list depending on the type, call its own cleanup routine */
	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"calling indexed list free routine.");
		if( ijmlist_idx_free(jml) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"calling the linked list free routine.");
		if( ijmlist_lnk_free(jml) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;
#ifdef WITH_ASSOC_LIST
	} else if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"calling the associative free routine...");
		if( ijmlist_ass_free(jml) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;
#endif
	} else
	{
		jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",(void*)jml,jml->flags);
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	/* free jmlist structure */
	
	free(jml);
	jmlist_debug(__func__, "jml=%p was freed successfuly",jml);
	
	jmlist_mem.total -= sizeof(struct _jmlist);
	jmlist_mem.used -= sizeof(struct _jmlist);
	jmlist_debug(__func__,"new jml_mem.total=%u",jmlist_mem.total);
	jmlist_debug(__func__,"new jml_mem.used=%u",jmlist_mem.used);
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   jmlist_push

   public function to insert an entry into the list.
   depeding on list type a specific routine will be called for each
   type.
*/
jmlist_status
jmlist_push(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p",jml,ptr);

	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list push routine.");
		return ijmlist_idx_push(jml,ptr);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list push routine.");
		return ijmlist_lnk_push(jml,ptr);
	}
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

/*
   jmlist_pop

   public function that pops an entry from the list. poping is the same of
   removing the entry from list and freeing it. its ptr is returned in ptr
   argument tho. depeding on the list type a specific function will be called
   for each list type.
*/
jmlist_status
jmlist_pop(jmlist jml,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p",jml,ptr);
	
	if( !ptr )
	{
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list pop routine.");
		return ijmlist_idx_pop(jml,ptr);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list pop routine.");
		return ijmlist_lnk_pop(jml,ptr);
	}
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

/*
   ijmlist_idx_ptr_exists
 
   checks if a specific ptr exists within the list.
   returns result in result argument which can be jmlist_entry_found
   or jmlist_entry_not_found.
*/
jmlist_status
ijmlist_idx_ptr_exists(jmlist jml,void *ptr,jmlist_lookup_result *result)
{
	jmlist_debug(__func__,"called with jml=%p and ptr=%p",jml,ptr);
	
	if( !result )
	{
		jmlist_debug(__func__,"invalid result pointer specified (result=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_index i;
	/* TODO: optimize this loop, if JMLIST_IDX_USE_SHIFT is on, we can look only within .usage entries */
	for( i = 0 ; i < jml->idx_list.capacity ; i++ )
	{
		if( jml->idx_list.plist[i] != ptr )
			continue;
		
		*result = jmlist_entry_found;
		jmlist_debug(__func__,"entry with ptr=%p was found in list jml=%p with index=%u",ptr,jml,i);
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	*result = jmlist_entry_not_found;
	jmlist_debug(__func__,"entry with ptr=%p was not found in list jml=%p with index=%u",ptr,jml,i);
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   ijmlist_lnk_ptr_exists
 
   verify if a specific ptr exists in a list. since its possible to have duplicate ptr values
   in the list, this function will return with result=jmlist_entry_found while there are
   entries with that ptr.
*/
jmlist_status
ijmlist_lnk_ptr_exists(jmlist jml,void *ptr,jmlist_lookup_result *result)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p",jml,ptr);

	if( !result )
	{
		jmlist_debug(__func__,"invalid result pointer specified (result=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"seeking ptr=%p in the linked list",ptr);
	
	linked_entry *pseeker = jml->lnk_list.phead;
	while( pseeker )
	{
		if( pseeker->ptr != ptr )
		{
			pseeker = pseeker->next;
			continue;
		}
		
		/* found entry */
		
		jmlist_debug(__func__,"found entry with ptr=%p on pseeker=%p in list jml=%p",ptr,pseeker,jml);
		*result = jmlist_entry_found;
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	*result = jmlist_entry_not_found;
	jmlist_debug(__func__,"couldn't find entry with ptr=%p in list jml=%p",ptr,jml);
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   jmlist_debug
 
   outputs a string to the debug channel set in jmlist initialization function.
*/
void jmlist_debug(const char *func,const char *fmt,...)
{
#ifdef ENABLE_DEBUG
	va_list args;
	
	va_start(args,fmt);
	fprintf(jmlist_cfg.fdebug,"%s: ",func);
	vfprintf(jmlist_cfg.fdebug,fmt,args);
	fprintf(jmlist_cfg.fdebug,"\n");
	va_end(args);
#else
	;
#endif
}

/* 
   jmlist_status_to_string
 
   convert status code to string, usefull for informative proposes.
*/
jmlist_status
jmlist_status_to_string(jmlist_status status,char *output,size_t output_len)
{
	switch (status) {
		case JMLIST_ERROR_SUCCESS:
			strncpy(output,"JMLIST_ERROR_SUCCESS",output_len);
			break;
		case JMLIST_ERROR_FAILURE:
			strncpy(output,"JMLIST_ERROR_FAILURE",output_len);
			break;
		case JMLIST_ERROR_DAMAGED_LIST:
			strncpy(output,"JMLIST_ERROR_DAMAGED_LIST",output_len);
			break;
		case JMLIST_ERROR_EMPTY_LIST:
			strncpy(output,"JMLIST_ERROR_EMPTY_LIST",output_len);
			break;
		case JMLIST_ERROR_ENTRY_NOT_FOUND:
			strncpy(output,"JMLIST_ERROR_ENTRY_NOT_FOUND",output_len);
			break;
		case JMLIST_ERROR_INVALID_ARGUMENT:
			strncpy(output,"JMLIST_ERROR_INVALID_ARGUMENT",output_len);
			break;
		case JMLIST_ERROR_MALLOC:
			strncpy(output,"JMLIST_ERROR_MALLOC",output_len);
			break;
		case JMLIST_ERROR_OUT_OF_BOUNDS:
			strncpy(output,"JMLIST_ERROR_OUT_OF_BOUNDS",output_len);
			break;
		case JMLIST_ERROR_UNIMPLEMENTED:
			strncpy(output,"JMLIST_ERROR_UNIMPLEMENTED",output_len);
			break;
		case JMLIST_ERROR_UNSUPPORTED:
			strncpy(output,"JMLIST_ERROR_UNSUPPORTED",output_len);
			break;
		default:
			strncpy(output,"(unknown status code!)",output_len);
			break;
	}
	return JMLIST_ERROR_SUCCESS;
}

/*
   jmlist_insert
   inserts a new entry into the list, this function requires only
   the ptr of the data. its specific for indexed and linked lists.
*/
jmlist_status
jmlist_insert(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p",jml,ptr);

	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list insert routine.");
		return ijmlist_idx_insert(jml,ptr);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list insert routine.");
		return ijmlist_lnk_insert(jml,ptr);
	}
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

/*
   jmlist_insert
   inserts a new entry into the list, this function requires the
   key_ptr and key_len besides the ptr of the data. its specific
   for associative lists type.
*/
jmlist_status
jmlist_insert_with_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p",jml,ptr);

#ifdef WITH_ASSOC_LIST	
	if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		return ijmlist_ass_insert(jml,key_ptr,key_len,ptr);
	}
#endif

	/* insert with key specification is only supported in associative list
	   types for now. */
	jmlist_debug(__func__,"unsuported call for this list (jml=%p) type",(void*)jml);

	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_UNSUPPORTED;
	return JMLIST_ERROR_FAILURE;
}

jmlist_status
jmlist_dump(jmlist jml)
{
	jmlist_debug(__func__,"called with jml=%p",jml);
		
	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list dump routine.");
		return ijmlist_idx_dump(jml);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list dump routine.");
		return ijmlist_lnk_dump(jml);
	}
#ifdef WITH_ASSOC_LIST
	 else if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"passing control to associative list dump routine.");
		return ijmlist_ass_dump(jml);
	}
#endif
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

/*
 * seeker will be used (causing this function to be O(N) instead of O(k)) when:
 *  - force_seeker = true
 *  - force_seeker = false and !JMLIST_IDX_USE_FRAG_FLAG
 *
 * you must set JMLIST_IDX_USE_FRAG_FLAG flag when creating idx list so this check
 * runs much faster, O(k). if this flag is set, when jmlist_remove_* and jmlist_insert_*
 * are called a condition verifies if a hole is being created or not, if yes, a bool
 * will be set to true on the list.
 */
jmlist_status
ijmlist_idx_is_fragmented(jmlist jml,bool force_seeker,bool *fragmented)
{
	jmlist_debug(__func__,"called with jml=%p, force_seeker=%u, fragmented=%p",jml,force_seeker,fragmented);
	
	if( !fragmented )
	{
		jmlist_debug(__func__,"invalid fragmented argument specified (fragmented=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	if( !force_seeker && (jml->flags & JMLIST_IDX_USE_FRAG_FLAG) )
	{
		*fragmented = jml->idx_list.fragmented;
		jmlist_debug(__func__,"using frag flag, gave the result %u",*fragmented);
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	/* seek throughout the list, hole is defined by x_i = 0, x_i+1 != 0 */
	jmlist_index i;
	for( i = 0 ; i < (jml->idx_list.capacity - 1) ; i++ )
	{
		if( (jml->idx_list.plist[i] == JMLIST_EMPTY_PTR) && 
				(jml->idx_list.plist[i+1] != JMLIST_EMPTY_PTR) )
		{
			jmlist_debug(__func__,"found hole in position %u",i);
			*fragmented = true;
			jmlist_debug(__func__,"returning with success.");
			return JMLIST_ERROR_SUCCESS;
		}
	}
	
	/* didn't found any holes.. list is not fragmented */
	*fragmented = false;
	jmlist_debug(__func__,"didn't found any holes, list is not fragmented");
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

jmlist_status
jmlist_is_fragmented(jmlist jml,bool force_seeker,bool *fragmented)
{
	jmlist_debug(__func__,"called with jml=%p, force_seeker=%u, fragmented=%p",jml,force_seeker,fragmented);
	
	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list is_fragmented routine.");
		return ijmlist_idx_is_fragmented(jml,force_seeker,fragmented);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"this function is unsuported in linked list type");
		jmlist_errno = JMLIST_ERROR_UNSUPPORTED;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

jmlist_status
jmlist_memory(jmlist_memory_info jml_mem)
{
	jmlist_debug(__func__,"called with jml_mem=%p",jml_mem);
	
	if( !jml_mem )
	{
		jmlist_debug(__func__,"invalid jml_mem argument specified (jml_mem=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* copy structure */
	memcpy(jml_mem,&jmlist_mem,sizeof(struct _jmlist_memory_info));
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

jmlist_status
jmlist_is_empty(jmlist jml,bool *empty)
{
	return JMLIST_ERROR_UNIMPLEMENTED;
}

jmlist_status
jmlist_free_all(void)
{
	jmlist_debug(__func__,"called");
	
	if( !(jmlist_cfg.flags & JMLIST_FLAG_INTERNAL_LIST) )
	{
		jmlist_debug(__func__,"cannot use this function when JMLIST_FLAG_INTERNAL_LIST is not set");
		jmlist_errno = JMLIST_ERROR_UNSUPPORTED;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* parse throughout the list of jmlists allocated and free all of them */
	
	void *ptr;
	while( jmlist_get_by_index(jmlist_ilist,0,&ptr) == JMLIST_ERROR_SUCCESS )
	{
		jmlist_debug(__func__,"detected unfreed list jml=%p tag=%s",ptr,((jmlist)ptr)->tag);
		
		jmlist_free((jmlist)ptr);
//		jmlist_remove_by_ptr(jmlist_ilist,ptr);
		
		/* calling jmlist_free will try to remove from internal list also, we don't want
		 that, so we call lnk_free directly. */
		//ijmlist_lnk_free((jmlist)ptr);
	}
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

#ifdef WITH_ASSOC_LIST
/*
 * associative list method get by index. associative list is nothing but an linked list
 * with a special parameter, the key, so an index access is equal in both lists.
 */
jmlist_status
ijmlist_ass_get_by_index(jmlist jml,jmlist_index index,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);
	
	if( !ptr )
	{
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	if( !jml->ass_list.usage )
	{
		jmlist_debug(__func__,"cannot get by index on empty list (ass_list.usage=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}
	
	/* check bounds! */
	if( index >= jml->ass_list.usage )
	{
		jmlist_debug(__func__,"index %u is out of bounds with list jml=%p which has %u entries",index,jml,jml->ass_list.usage);
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"parsing associative list for indexed item %u",index);
	
	jmlist_index i = 0;
	assoc_entry *pseeker = jml->ass_list.phead;
	/* Fix: when pseeker = 0, trow error! */
	while( i != index )
	{
		pseeker = pseeker->next;
		i++;
	}

	*ptr = pseeker->ptr;
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}
#endif

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_insert(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p and ptr=%p",jml,ptr);
	
	/* verify key_ptr */
	if( !key_ptr )
	{
		jmlist_debug(__func__,"invalid key_ptr specified (key_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* verify key_len */
	if( !key_len )
	{
		jmlist_debug(__func__,"invalid key_len specified (key_len=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	if( (jml->flags & JMLIST_ASS_INSERT_AT_TAIL) )
	{
		jmlist_debug(__func__,"inserting at tail");
		
		assoc_entry *pseeker = jml->ass_list.phead;
		while( pseeker->next ) pseeker = pseeker->next;
		
		/* found entry */
		jmlist_debug(__func__,"found tail at pseeker=%p",pseeker);
		
		jmlist_debug(__func__,"allocating linked entry");
		assoc_entry *pentry = (assoc_entry*)malloc(sizeof(assoc_entry));
		if( !pentry ) 
		{
			jmlist_debug(__func__,"malloc failed (size %u)!",sizeof(assoc_entry));
			jmlist_errno = JMLIST_ERROR_MALLOC;
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
		jmlist_debug(__func__,"new associative entry pentry=%p allocated",pentry);
		
		/* initialize entry */
		pentry->ptr = ptr;
		pentry->key_ptr = key_ptr;
		pentry->key_len = key_len;
		pentry->next = 0;
		jmlist_debug(__func__,"initialized new entry successfuly (ptr=%p, next=%p, key=%p, len=%u)",
					 pentry->ptr,pentry->next,pentry->key_ptr,pentry->key_len);
		
		pseeker->next = pentry;
		
		jml->ass_list.usage++;
		jmlist_debug(__func__,"new usage of list is %u",jml->ass_list.usage);
		
		jmlist_mem.ass_list.total += sizeof(assoc_entry);
		jmlist_mem.ass_list.used += sizeof(assoc_entry);
		jmlist_debug(__func__,"new jml_mem.ass_list.total is %u",jmlist_mem.ass_list.total);
		jmlist_debug(__func__,"new jml_mem.ass_list.used is %u",jmlist_mem.ass_list.used);
		
	} else
	{
		jmlist_debug(__func__,"allocating associative entry");
		assoc_entry *pentry = (assoc_entry*)malloc(sizeof(assoc_entry));
		if( !pentry ) 
		{
			jmlist_debug(__func__,"malloc failed (size %u)!",sizeof(assoc_entry));
			jmlist_errno = JMLIST_ERROR_MALLOC;
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
		jmlist_debug(__func__,"new associative entry pentry=%p allocated",pentry);
		
		jmlist_mem.ass_list.total += sizeof(struct _assoc_entry);
		jmlist_mem.ass_list.used += sizeof(struct _assoc_entry);
		jmlist_debug(__func__,"new jml_mem.ass_list.total is %u",jmlist_mem.ass_list.total);
		jmlist_debug(__func__,"new jml_mem.ass_list.used is %u",jmlist_mem.ass_list.used);
		
		/* initialize entry */
		pentry->ptr = ptr;
		pentry->next = jml->ass_list.phead;
		pentry->key_ptr = key_ptr;
		pentry->key_len = key_len;
		jmlist_debug(__func__,"initialized new entry successfuly (ptr=%p, next=%p, key_ptr=%p, key_len=%u)",
					 pentry->ptr,pentry->next,pentry->key_ptr,pentry->key_len);
		
		jmlist_debug(__func__,"inserting pentry in head of list (current head phead=%p, usage=%u)",
					 jml->ass_list.phead,jml->ass_list.usage);
		
		jml->ass_list.phead = pentry;
		jml->ass_list.usage++;
		
		jmlist_debug(__func__,"now head is phead=%p with phead->next=%p (usage=%u)",
					 jml->ass_list.phead,pentry->next,jml->ass_list.usage);
	} // (jml->flags & JMLIST_ASS_INSERT_AT_TAIL)
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;	
}
#endif

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_remove_by_ptr(jmlist jml,void *ptr)
{
	jmlist_debug(__func__,"called with jml=%p and ptr=%p\n",(void*)jml,(void*)ptr);

	/* trying to pop from empty list? */
	if( !jml->ass_list.usage )
	{
		jmlist_debug(__func__,"trying to remove from empty list");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"seeking ptr=%p in the associative list",ptr);
	
	assoc_entry *pseeker = jml->ass_list.phead;
	assoc_entry *pprevious = pseeker;
	while( pseeker )
	{
		if( pseeker->ptr != ptr )
		{
			pprevious = pseeker;
			pseeker = pseeker->next;
			continue;
		}
		
		jmlist_debug(__func__,"found entry, pseeker=%p pseeker->next=%p",pseeker,pseeker->next);
		
		if( pprevious == pseeker )
		{
			jmlist_debug(__func__,"entry is from head, lets pop it instead...");

			assoc_entry *pentry;
			pentry = jml->ass_list.phead;
			
			jmlist_debug(__func__,"entry poped from head is jml=%p",pentry);

			jml->ass_list.phead = pentry->next;

			jmlist_debug(__func__,"new list head is phead=%p",pentry->next);
			jmlist_debug(__func__,"freeing pentry=%p",pentry);
			
			free(pentry);
			
			jml->ass_list.usage--;
			jmlist_debug(__func__,"decreased usage to %u",jml->ass_list.usage);
			
			jmlist_mem.ass_list.total -= sizeof(struct _assoc_entry);
			jmlist_mem.ass_list.used -= sizeof(struct _assoc_entry);
			jmlist_debug(__func__,"new jml_mem.ass_list.total is %u",jmlist_mem.ass_list.total);
			jmlist_debug(__func__,"new jml_mem.ass_list.used is %u",jmlist_mem.ass_list.used);
			
			jmlist_debug(__func__,"returning with success.");
			return JMLIST_ERROR_SUCCESS;
		}
		
		/* we're not at HEAD, remove entry normally... */
		jmlist_debug(__func__,"unlinking entry");
		pprevious->next = pseeker->next;
		jml->ass_list.usage--;
		
		jmlist_mem.ass_list.total -= sizeof(struct _assoc_entry);
		jmlist_mem.ass_list.used -= sizeof(struct _assoc_entry);
		jmlist_debug(__func__,"new jml_mem.ass_list.total is %u",jmlist_mem.ass_list.total);
		jmlist_debug(__func__,"new jml_mem.ass_list.used is %u",jmlist_mem.ass_list.used);
		
		
		jmlist_debug(__func__,"unlinked successfuly, new usage is %u, freeing entry",jml->ass_list.usage);
		free(pseeker);
		
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	jmlist_debug(__func__,"couldn't find entry with ptr=%p in the list jml=%p",ptr,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}
#endif

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_dump(jmlist jml)
{
	jmlist_debug(__func__,"called with jml=%p",jml);
	
	fprintf(jmlist_cfg.fdump,"associative list jml=%p has usage=%u and phead=%p\n",
			(void*)jml,jml->ass_list.usage,(void*)jml->ass_list.phead);
	
	assoc_entry *pseeker = jml->ass_list.phead;
	
	if( pseeker )
		fprintf(jmlist_cfg.fdump,"list of entries:\n");
	
	jmlist_index index = 0;
	bool printable_key;
	jmlist_index i;
	while( pseeker )
	{
		/* if key has only printable chars, dump key as ASCII otherwise just
		   print the pointer and length. */
		printable_key = true;
		for( i = 0 ; i < pseeker->key_len ; i++ )
		{
			if( isprint(((unsigned char*)pseeker->key_ptr)[i]) )
				continue;

			printable_key = false;
		}
		
		if( printable_key )
		{
			fprintf(jmlist_cfg.fdump,"  %08X: pentry=%p pentry->next=%p key=<",
					index,(void*)pseeker,(void*)pseeker->next);

			for( i = 0 ; i < pseeker->key_len ; i++ )
				fprintf(jmlist_cfg.fdump,"%c",((char*)pseeker->key_ptr)[i]);

			fprintf(jmlist_cfg.fdump,"> key_len=%u ptr=%p\n",pseeker->key_len,pseeker->ptr);
		} else
		{
			fprintf(jmlist_cfg.fdump,"  %08X: pentry=%p pentry->next=%p key=%p key_len=%u ptr=%p\n",
					index,(void*)pseeker,(void*)pseeker->next,(void*)pseeker->key_ptr,pseeker->key_len,pseeker->ptr);
		}
		pseeker = pseeker->next;
		index++;
	}
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}
#endif

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_free(jmlist jml)
{
	jmlist_debug(__func__,"called with jml=%p",jml);
	
	assoc_entry *pseeker = jml->ass_list.phead;
	assoc_entry *pnext;
	while( pseeker )
	{
		pnext = pseeker->next;
		jmlist_debug(__func__,"freeing associative entry with p=%p, next is p=%p",pseeker,pnext);

		free(pseeker);
		
		jmlist_debug(__func__,"freed associative entry sucessfuly with p=%p",pseeker);
		
		jmlist_mem.ass_list.total -= sizeof(struct _assoc_entry);
		jmlist_mem.ass_list.used -= sizeof(struct _assoc_entry);
		jmlist_debug(__func__,"new jml_mem.ass_list.total is %u",jmlist_mem.ass_list.total);
		jmlist_debug(__func__,"new jml_mem.ass_list.used is %u",jmlist_mem.ass_list.used);
		pseeker = pnext;
	}
	
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}
#endif

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_ptr_exists(jmlist jml,void *ptr,jmlist_lookup_result *result)
{
	jmlist_debug(__func__,"called with jml=%p, ptr=%p, result=%p",jml,ptr,result);
	
	if( !result )
	{
		jmlist_debug(__func__,"invalid result pointer specified (result=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	/* trying ptr_exists on an empty list? */
	if( !jml->ass_list.usage )
	{
		jmlist_debug(__func__,"cannot use this function with an empty list");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"seeking ptr=%p in the linked list",ptr);
	
	assoc_entry *pseeker = jml->ass_list.phead;
	while( pseeker )
	{
		if( pseeker->ptr != ptr )
		{
			pseeker = pseeker->next;
			continue;
		}
		
		/* found entry */
		jmlist_debug(__func__,"found entry with ptr=%p on pseeker=%p in list jml=%p",ptr,pseeker,jml);
		*result = jmlist_entry_found;
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	*result = jmlist_entry_not_found;
	jmlist_debug(__func__,"couldn't find entry with ptr=%p in list jml=%p",ptr,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}
#endif

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_key_exists(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len,jmlist_lookup_result *result)
{
	jmlist_debug(__func__,"called with jml=%p, key_ptr=%p, key_len=%u, result=%p",jml,key_ptr,key_len,result);
	
	if( !result )
	{
		jmlist_debug(__func__,"invalid result pointer specified (result=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}
	
	/* verify key_ptr */
	if( !key_ptr )
	{
		jmlist_debug(__func__,"invalid key_ptr specified (key_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* verify key_len */
	if( !key_len )
	{
		jmlist_debug(__func__,"invalid key_len specified (key_len=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* trying ptr_exists on an empty list? */
	if( !jml->ass_list.usage )
	{
		jmlist_debug(__func__,"cannot use this function with an empty list");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"seeking key_ptr=%p, key_len=%u in the linked list",key_ptr,key_len);
	
	assoc_entry *pseeker = jml->ass_list.phead;
	while( pseeker )
	{
		if( (pseeker->key_len != key_len) || memcmp(pseeker->key_ptr,key_ptr,key_len) )
		{
			pseeker = pseeker->next;
			continue;
		}
		
		/* found entry */
		jmlist_debug(__func__,"found entry of key_ptr %p (length %u) on pseeker=%p in list jml=%p",key_ptr,key_len,pseeker,jml);
		*result = jmlist_entry_found;
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	*result = jmlist_entry_not_found;
	jmlist_debug(__func__,"couldn't find entry with key_ptr=%p and key_len=%u in list jml=%p",key_ptr,key_len,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}
#endif

/*
 * get method for lists that have keys associated.
 */
jmlist_status
jmlist_get_by_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p, key_ptr=%p, key_len=%u, ptr=%p",jml,key_ptr,key_len,ptr);
	
	if( !ptr )
	{
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		return JMLIST_ERROR_FAILURE;
	}

	/* verify key_ptr */
	if( !key_ptr )
	{
		jmlist_debug(__func__,"invalid key_ptr specified (key_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* verify key_len */
	if( !key_len )
	{
		jmlist_debug(__func__,"invalid key_len specified (key_len=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	/* for now this function is only supported in associative lists */	
#ifdef WITH_ASSOC_LIST
	if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"passing control to the associative get by key routine.");
		return ijmlist_ass_get_by_key(jml,key_ptr,key_len,ptr);
	}
#endif

	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_get_by_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p key_ptr=%p key_len=%u ptr=%p",jml,key_ptr,key_len,ptr);

	/* arguments check, this should be already done by the calling function, tho you
	   can repeat it here if you want... */

	DCHECKSTART
	/* verify key_ptr */
	if( !key_ptr )
	{
		jmlist_debug(__func__,"invalid key_ptr specified (key_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* verify key_len */
	if( !key_len )
	{
		jmlist_debug(__func__,"invalid key_len specified (key_len=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	DCHECKEND

	jmlist_debug(__func__,"seeking key_ptr=%p in the linked list",key_ptr);
	
	assoc_entry *pseeker = jml->ass_list.phead;
	while( pseeker )
	{
		if( (pseeker->key_len != key_len) || memcmp(pseeker->key_ptr,key_ptr,key_len) )
		{
			pseeker = pseeker->next;
			continue;
		}
		
		/* found entry */
		jmlist_debug(__func__,"found entry of key_ptr %p (length %u) on pseeker=%p in list jml=%p",
					 key_ptr,key_len,pseeker,jml);
		*ptr = pseeker->ptr;
		
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	jmlist_debug(__func__,"couldn't find entry with key_ptr=%p key_len=%u in list jml=%p",
				 key_ptr,key_len,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}
#endif

/*
   jmlist_remove_by_key
   removes an entry from the list using the key for the lists that actually support keys.
*/
jmlist_status
jmlist_remove_by_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len)
{
	jmlist_debug(__func__,"called with jml=%p key_ptr=%p key_len=%u",jml,key_ptr,key_len);
	
	/* verify key_ptr */
	if( !key_ptr )
	{
		jmlist_debug(__func__,"invalid key_ptr specified (key_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}
	
	/* verify key_len */
	if( !key_len )
	{
		jmlist_debug(__func__,"invalid key_len specified (key_len=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

#ifdef WITH_ASSOC_LIST
	if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"passing control to associative list remove by key routine.");
		return ijmlist_ass_remove_by_key(jml,key_ptr,key_len);
	}
#endif

	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_remove_by_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len)
{
	jmlist_debug(__func__,"called with jml=%p key_ptr=%p key_len=%u\n",(void*)jml,(void*)key_ptr,key_len);

	/* trying to pop from empty list? */
	if( !jml->ass_list.usage )
	{
		jmlist_debug(__func__,"trying to remove from empty list");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"seeking key_ptr=%p key_len=%u in the associative list",key_ptr,key_len);
	
	assoc_entry *pseeker = jml->ass_list.phead;
	assoc_entry *pprevious = pseeker;
	while( pseeker )
	{
		if( (pseeker->key_len != key_len) || (memcmp(pseeker->key_ptr,key_ptr,key_len)) )
		{
			pprevious = pseeker;
			pseeker = pseeker->next;
			continue;
		}
		
		jmlist_debug(__func__,"found entry, pseeker=%p pseeker->next=%p",pseeker,pseeker->next);
		
		if( pprevious == pseeker )
		{
			jmlist_debug(__func__,"entry is from head, lets pop it instead...");

			assoc_entry *pentry;
			pentry = jml->ass_list.phead;
			
			jmlist_debug(__func__,"entry poped from head is jml=%p",pentry);

			jml->ass_list.phead = pentry->next;

			jmlist_debug(__func__,"new list head is phead=%p",pentry->next);
			jmlist_debug(__func__,"freeing pentry=%p",pentry);
			
			free(pentry);
			
			jml->ass_list.usage--;
			jmlist_debug(__func__,"decreased usage to %u",jml->ass_list.usage);
			
			jmlist_mem.ass_list.total -= sizeof(struct _assoc_entry);
			jmlist_mem.ass_list.used -= sizeof(struct _assoc_entry);
			jmlist_debug(__func__,"new jml_mem.ass_list.total is %u",jmlist_mem.ass_list.total);
			jmlist_debug(__func__,"new jml_mem.ass_list.used is %u",jmlist_mem.ass_list.used);
			
			jmlist_debug(__func__,"returning with success.");
			return JMLIST_ERROR_SUCCESS;
		}
		
		/* we're not at HEAD, remove entry normally... */
		jmlist_debug(__func__,"unlinking entry");
		pprevious->next = pseeker->next;
		jml->ass_list.usage--;
		
		jmlist_mem.ass_list.total -= sizeof(struct _assoc_entry);
		jmlist_mem.ass_list.used -= sizeof(struct _assoc_entry);
		jmlist_debug(__func__,"new jml_mem.ass_list.total is %u",jmlist_mem.ass_list.total);
		jmlist_debug(__func__,"new jml_mem.ass_list.used is %u",jmlist_mem.ass_list.used);
		
		
		jmlist_debug(__func__,"unlinked successfuly, new usage is %u, freeing entry",jml->ass_list.usage);
		free(pseeker);
		
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	jmlist_debug(__func__,"couldn't find entry with key_ptr=%p key_len=%u in list jml=%p",key_ptr,key_len,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}
#endif

jmlist_status
jmlist_entry_count(jmlist jml,jmlist_index *entry_count)
{
	jmlist_debug(__func__,"called with jml=%p, entry_count=%p");

	if( !entry_count )
	{
		jmlist_debug(__func__,"invalid entry_count argument (entry_count=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"updating entry_count to %u",jml->idx_list.usage);
		*entry_count = jml->idx_list.usage;
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"updating entry_count to %u",jml->lnk_list.usage);
		*entry_count = jml->lnk_list.usage;
	}

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   jmlist_remove_by_index

   Removes an entry from the list using the entry index.
*/
jmlist_status
jmlist_remove_by_index(jmlist jml,jmlist_index index)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);
	
	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list remove_by_index routine.");
		return ijmlist_idx_remove_by_index(jml,index);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list remove_by_index routine.");
		return ijmlist_lnk_remove_by_index(jml,index);
	}
#ifdef WITH_ASSOC_LIST
	else if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"passing control to the associative list remove_by_index routine.");
		return ijmlist_ass_remove_by_index(jml,index);
	}
#endif
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

jmlist_status
ijmlist_idx_remove_by_index(jmlist jml,jmlist_index index)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);

	if( !jml->idx_list.usage )
	{
		jmlist_debug(__func__,"cannot get by index on empty list (lnk_list.usage=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}

	/* shouldn't use get on a fragmented idx list */
	if( !(jml->flags & JMLIST_IDX_USE_SHIFT) )
	{
		/* check for fragmentation */
		bool fragmented = false;
		if( jmlist_is_fragmented(jml,false,&fragmented) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;

		if( fragmented == true )
		{
			jmlist_debug(__func__,"cannot use this function with possible fragmented indexed lists, use of JMLIST_IDX_USE_SHIFT is required");
			jmlist_errno = JMLIST_ERROR_FAILURE;
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
	}

	/* check bounds */
	if( index >= jml->idx_list.usage )
	{
		jmlist_debug(__func__,"index %u is out of bounds with list jml=%p which has %u entries",index,jml,jml->idx_list.usage);
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure");
		return JMLIST_ERROR_FAILURE;
	}

	/* remove the entry... */
	jmlist_debug(__func__,"setting entry ptr (now %p) to NULL from list %p in index %u",jml->idx_list.plist[index],jml,index);

	/* found the entry, clear it */
	jml->idx_list.plist[index] = JMLIST_EMPTY_PTR;
	jml->idx_list.usage--;

	jmlist_mem.idx_list.used -= sizeof(void*);
	jmlist_debug(__func__,"new jml_mem.idx_list.used is %u",jmlist_mem.idx_list.used);

	jmlist_debug(__func__,"entry with index %u removed from list %p, new usage is %u",index,jml,jml->idx_list.usage);

	/* if shift is activated, remove and shift, otherwise just remove */
	if( jml->flags & JMLIST_IDX_USE_SHIFT )
	{
		jmlist_debug(__func__,"JMLIST_IDX_USE_SHIFT activated, shifting list entries.");

		for( unsigned int i = index ; i < (jml->idx_list.capacity - 1) ; i++ )
			jml->idx_list.plist[i] = jml->idx_list.plist[i+1];
	}

	if( (jml->flags & JMLIST_IDX_USE_FRAG_FLAG) && (jml->idx_list.fragmented == false) )
	{
		if( (index < (jml->idx_list.capacity-1)) && (jml->idx_list.plist[index+1] != JMLIST_EMPTY_PTR) )
			jml->idx_list.fragmented = true;

		/* else we'll keep it in false (not fragmented) */
	}

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

jmlist_status
ijmlist_lnk_remove_by_index(jmlist jml,jmlist_index index)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);

	if( !jml->lnk_list.usage )
	{
		jmlist_debug(__func__,"trying to remove from empty list");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}

	/* check bounds! */
	if( index >= jml->lnk_list.usage )
	{
		jmlist_debug(__func__,"index %u is out of bounds with list jml=%p which has %u entries",index,jml,jml->lnk_list.usage);
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure");
		return JMLIST_ERROR_FAILURE;
	}

	jmlist_debug(__func__,"seeking for index=%u in the linked list",index);

	jmlist_index index_seeker = 0;	
	linked_entry *pseeker = jml->lnk_list.phead;
	linked_entry *pprevious = pseeker;
	while( pseeker )
	{
		if( index_seeker != index )
		{
			pprevious = pseeker;
			pseeker = pseeker->next;
			index_seeker++;
			continue;
		}

		jmlist_debug(__func__,"found entry with index=%u, pseeker=%p pseeker->next=%p",index,pseeker,pseeker->next);

		if( pprevious == pseeker )
		{
			/* we're at HEAD */
			jmlist_debug(__func__,"entry is from head, lets pop it instead...");
			void *junk;
			return ijmlist_lnk_pop(jml,&junk);
		}

		jmlist_debug(__func__,"unlinking entry with index=%u",index);
		pprevious->next = pseeker->next;
		jml->lnk_list.usage--;

		jmlist_debug(__func__,"unlinked successfuly, new usage is %u, freeing entry structure",jml->lnk_list.usage);
		free(pseeker);

		jmlist_mem.lnk_list.total -= sizeof(struct _linked_entry);
		jmlist_mem.lnk_list.used -= sizeof(struct _linked_entry);
		jmlist_debug(__func__,"new jml_mem.lnk_list.total is %u",jmlist_mem.lnk_list.total);
		jmlist_debug(__func__,"new jml_mem.lnk_list.used is %u",jmlist_mem.lnk_list.used);

		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}

	/* this shouldn't happen since the bounds were checked before! damaged list?? */
	jmlist_debug(__func__,"unexpected error! couldn't find entry with index=%u in list jml=%p",index,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_remove_by_index(jmlist jml,jmlist_index index)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);
	
	if( !jml->ass_list.usage )
	{
		jmlist_debug(__func__,"cannot remove on empty list");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}
	
	/* check bounds! */
	if( index >= jml->ass_list.usage )
	{
		jmlist_debug(__func__,"index %u is out of bounds with list jml=%p which has %u entries",index,jml,jml->ass_list.usage);
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"seeking associative list for indexed item %u",index);
	
	jmlist_index index_seeker = 0;
	assoc_entry *pseeker = jml->ass_list.phead;
	assoc_entry *pprevious = pseeker;
	while( pseeker )
	{
		if( index_seeker != index )
		{
			pprevious = pseeker;
			pseeker = pseeker->next;
			index_seeker++;
			continue;
		}
		
		jmlist_debug(__func__,"found entry with index=%u, pseeker=%p pseeker->next=%p",index,pseeker,pseeker->next);
		
		if( pprevious == pseeker )
		{
			jmlist_debug(__func__,"entry is from head, lets pop it instead...");
			
			assoc_entry *pentry;
			pentry = jml->ass_list.phead;
			
			jmlist_debug(__func__,"entry poped from head is jml=%p",pentry);
			
			jml->ass_list.phead = pentry->next;
			
			jmlist_debug(__func__,"new list head is phead=%p",pentry->next);
			jmlist_debug(__func__,"freeing pentry=%p structure from memory",pentry);
			
			free(pentry);
			
			jml->ass_list.usage--;
			jmlist_debug(__func__,"decreased usage to %u",jml->ass_list.usage);
			
			jmlist_mem.ass_list.total -= sizeof(struct _assoc_entry);
			jmlist_mem.ass_list.used -= sizeof(struct _assoc_entry);
			jmlist_debug(__func__,"new jml_mem.ass_list.total is %u",jmlist_mem.ass_list.total);
			jmlist_debug(__func__,"new jml_mem.ass_list.used is %u",jmlist_mem.ass_list.used);
			
			jmlist_debug(__func__,"returning with success.");
			return JMLIST_ERROR_SUCCESS;
		}
		
		/* we're not at HEAD, remove entry normally... */
		jmlist_debug(__func__,"unlinking entry");
		pprevious->next = pseeker->next;
		jml->ass_list.usage--;
		
		jmlist_mem.ass_list.total -= sizeof(struct _assoc_entry);
		jmlist_mem.ass_list.used -= sizeof(struct _assoc_entry);
		jmlist_debug(__func__,"new jml_mem.ass_list.total is %u",jmlist_mem.ass_list.total);
		jmlist_debug(__func__,"new jml_mem.ass_list.used is %u",jmlist_mem.ass_list.used);
		
		jmlist_debug(__func__,"unlinked successfuly, new usage is %u, freeing entry",jml->ass_list.usage);
		free(pseeker);
		
		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	jmlist_debug(__func__,"couldn't find entry with index=%u in the list jml=%p",index,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}
#endif

/*
   jmlist_replace_by_index

   In indexed lists, entry is of index=index is cleared and replaced with new_ptr, this is
   done ignoring the indexed list flags of shifting and fragmented. In the case of linked
   lists the next pointer of the previous entry is replaced by new_ptr and new_ptr->next
   is set to old previous->next. Associative lists have equal behaviour of linked ones.

   In all the 3 cases, no memory counters are changed.

   The difference from replace and remove+insert is that the new_ptr will maintain the
   index specified by the argument in all the 3 types of lists. Also replace is faster
   because it doesn't do operations associated with insert or remove.
*/
jmlist_status
jmlist_replace_by_index(jmlist jml,jmlist_index index,void *new_ptr)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u, new_ptr=%p",jml,index,new_ptr);
	
	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list replace_by_index routine.");
		return ijmlist_idx_replace_by_index(jml,index,new_ptr);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list replace_by_index routine.");
		return ijmlist_lnk_replace_by_index(jml,index,new_ptr);
	}
#ifdef WITH_ASSOC_LIST
	else if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"passing control to the associative list replace_by_index routine.");
		return ijmlist_ass_replace_by_index(jml,index,new_ptr);
	}
#endif
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

jmlist_status
ijmlist_idx_replace_by_index(jmlist jml,jmlist_index index,void *new_ptr)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);

	if( !jml->idx_list.usage )
	{
		jmlist_debug(__func__,"cannot replace by index on empty list (lnk_list.usage=0)");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}

	/* shouldn't use get on a fragmented idx list */
	if( !(jml->flags & JMLIST_IDX_USE_SHIFT) )
	{
		/* check for fragmentation */
		bool fragmented = false;
		if( jmlist_is_fragmented(jml,false,&fragmented) == JMLIST_ERROR_FAILURE )
			return JMLIST_ERROR_FAILURE;

		if( fragmented == true )
		{
			jmlist_debug(__func__,"cannot use this function with possible fragmented indexed lists, use of JMLIST_IDX_USE_SHIFT is required");
			jmlist_errno = JMLIST_ERROR_FAILURE;
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
	}

	/* check bounds */
	if( index >= jml->idx_list.usage )
	{
		jmlist_debug(__func__,"index %u is out of bounds with list jml=%p which has %u entries",index,jml,jml->idx_list.usage);
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure");
		return JMLIST_ERROR_FAILURE;
	}

	/* remove the entry... */
	jmlist_debug(__func__,"setting entry ptr (now %p) to new_ptr=%p from list %p in index %u",jml->idx_list.plist[index],new_ptr,jml,index);

	/* found the entry, replace it */
	jml->idx_list.plist[index] = new_ptr;

	jmlist_debug(__func__,"entry with index %u replaced from list %p successfully, new entry has ptr=%p",index,jml,jml->idx_list.plist[index]);
	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

jmlist_status
ijmlist_lnk_replace_by_index(jmlist jml,jmlist_index index,void *new_ptr)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);

	if( !jml->lnk_list.usage )
	{
		jmlist_debug(__func__,"trying to replace from empty list");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}

	/* check bounds! */
	if( index >= jml->lnk_list.usage )
	{
		jmlist_debug(__func__,"index %u is out of bounds with list jml=%p which has %u entries",index,jml,jml->lnk_list.usage);
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure");
		return JMLIST_ERROR_FAILURE;
	}

	jmlist_debug(__func__,"seeking for index=%u in the linked list",index);

	jmlist_index index_seeker = 0;	
	linked_entry *pseeker = jml->lnk_list.phead;
	linked_entry *pprevious = pseeker;
	while( pseeker )
	{
		if( index_seeker != index )
		{
			pprevious = pseeker;
			pseeker = pseeker->next;
			index_seeker++;
			continue;
		}

		jmlist_debug(__func__,"found entry with index=%u, pseeker=%p pseeker->next=%p",index,pseeker,pseeker->next);

		jmlist_debug(__func__,"replacing old ptr=%p with new_ptr=%p",pseeker->ptr,new_ptr);
		pseeker->ptr = new_ptr;
		jmlist_debug(__func__,"new entry ptr of index=%u is ptr=%p",index,pseeker->ptr);

		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}

	/* this shouldn't happen since the bounds were checked before! damaged list?? */
	jmlist_debug(__func__,"unexpected error! couldn't find entry with index=%u in list jml=%p",index,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_replace_by_index(jmlist jml,jmlist_index index,void *new_ptr)
{
	jmlist_debug(__func__,"called with jml=%p, index=%u",jml,index);
	
	if( !jml->ass_list.usage )
	{
		jmlist_debug(__func__,"cannot remove on empty list");
		jmlist_debug(__func__,"returning with failure.");
		jmlist_errno = JMLIST_ERROR_EMPTY_LIST;
		return JMLIST_ERROR_FAILURE;
	}
	
	/* check bounds! */
	if( index >= jml->ass_list.usage )
	{
		jmlist_debug(__func__,"index %u is out of bounds with list jml=%p which has %u entries",index,jml,jml->ass_list.usage);
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure");
		return JMLIST_ERROR_FAILURE;
	}
	
	jmlist_debug(__func__,"seeking associative list for indexed item %u",index);
	
	jmlist_index index_seeker = 0;
	assoc_entry *pseeker = jml->ass_list.phead;
	assoc_entry *pprevious = pseeker;
	while( pseeker )
	{
		if( index_seeker != index )
		{
			pprevious = pseeker;
			pseeker = pseeker->next;
			index_seeker++;
			continue;
		}
		
		jmlist_debug(__func__,"found entry with index=%u, pseeker=%p pseeker->next=%p",index,pseeker,pseeker->next);

		jmlist_debug(__func__,"replacing old ptr=%p with new_ptr=%p",pseeker->ptr,new_ptr);
		pseeker->ptr = new_ptr;
		jmlist_debug(__func__,"new entry ptr of index=%u is ptr=%p",index,pseeker->ptr);

		jmlist_debug(__func__,"returning with success.");
		return JMLIST_ERROR_SUCCESS;
	}
	
	jmlist_debug(__func__,"couldn't find entry with index=%u in the list jml=%p",index,jml);
	jmlist_errno = JMLIST_ERROR_ENTRY_NOT_FOUND;
	jmlist_debug(__func__,"returning with failure.");
	return JMLIST_ERROR_FAILURE;
}
#endif

/*
   jmlist_seek_start

   Start seeking in a list, the fastest seeking process is different depending on the
   list type thats why jmlist_seek_* routines were coded. There should be passed a pointer
   to a seek_handle_t structure memory space which will be used to identify the seeking.

   Only single-seeking is supported at the time, a seeking finishes by calling the
   function jmlist_seek_end. That will allow other seeks to be done.
*/
jmlist_status
jmlist_seek_start(jmlist jml,jmlist_seek_handle *handle_ptr)
{
	jmlist_debug(__func__,"called with jml=%p and handle=%p",jml,handle_ptr);

	if( !jml )
	{
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr ) {
		jmlist_debug(__func__,"invalid handle pointer specified (handle_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( jml->seeking == true ) {
		jmlist_debug(__func__,"multiple concorrent seeks are not supported yet");
		jmlist_errno = JMLIST_ERROR_UNSUPPORTED;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	/* set seeking flag */
	jml->seeking = true;
	jmlist_debug(__func__,"seek flag was set to %u",jml->seeking);

	/* clear handle data for the specific list type */

	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list seek_init routine.");
		return ijmlist_idx_seek_start(jml,handle_ptr);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list seek_init routine.");
		return ijmlist_lnk_seek_start(jml,handle_ptr);
	}
#ifdef WITH_ASSOC_LIST
	else if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"passing control to the associative list seek_init routine.");
		return ijmlist_ass_seek_start(jml,handle_ptr);
	}
#endif
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

jmlist_status
ijmlist_idx_seek_start(jmlist jml,jmlist_seek_handle *handle_ptr)
{
	jmlist_debug(__func__,"called with jml=%p and handle=%p",jml,handle_ptr);

	if( !jml )
	{
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr ) {
		jmlist_debug(__func__,"invalid handle pointer specified (handle_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	handle_ptr->next_idx = 0;
	jmlist_debug(__func__,"handle index set to %u",handle_ptr->next_idx);

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

jmlist_status
ijmlist_lnk_seek_start(jmlist jml,jmlist_seek_handle *handle_ptr)
{
	jmlist_debug(__func__,"called with jml=%p and handle=%p",jml,handle_ptr);

	if( !jml )
	{
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr ) {
		jmlist_debug(__func__,"invalid handle pointer specified (handle_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	handle_ptr->next_lnk = jml->lnk_list.phead;
	jmlist_debug(__func__,"handle next_lnk set to %p",handle_ptr->next_lnk);

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_seek_start(jmlist jml,jmlist_seek_handle *handle_ptr)
{
	jmlist_debug(__func__,"called with jml=%p and handle=%p",jml,handle_ptr);

	if( !jml )
	{
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr ) {
		jmlist_debug(__func__,"invalid handle pointer specified (handle_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	handle_ptr->next_ass = jml->ass_list.phead;
	jmlist_debug(__func__,"handle next_ass set to %p",handle_ptr->next_ass);

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}
#endif

jmlist_status
jmlist_seek_end(jmlist jml,jmlist_seek_handle *handle_ptr)
{
	jmlist_debug(__func__,"called with jml=%p and handle_ptr=%p",jml,handle_ptr);

	if( !jml ) {
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr ) {
		jmlist_debug(__func__,"invalid handle_ptr specified (handle_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !jml->seeking ) {
		jmlist_debug(__func__,"cannot stop seeking on a list that is not being seeked");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	jml->seeking = false;
	jmlist_debug(__func__,"seek flag set to %u",jml->seeking);

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   jmlist_seek_next

   Parse list and obtain the next entry, an argument, handler structure
   is used to determine where the seeking is at. Each list type have its own
   handler data.
*/
jmlist_status
jmlist_seek_next(jmlist jml,jmlist_seek_handle *handle_ptr,void **ptr)
{
	jmlist_debug(__func__,"called with jml=%p, handle_ptr=%p and ptr=%p",jml,handle_ptr,ptr);

	if( !jml )
	{
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !ptr ) {
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr ) {
		jmlist_debug(__func__,"invalid handle_ptr specified (handle_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( jml->seeking != true ) {
		jmlist_debug(__func__,"seek should start by calling seek_start routine");
		jmlist_errno = JMLIST_ERROR_FAILURE;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( jml->flags & JMLIST_INDEXED )
	{
		jmlist_debug(__func__,"passing control to indexed list seek_next routine.");
		return ijmlist_idx_seek_next(jml,handle_ptr,ptr);
	} else if( jml->flags & JMLIST_LINKED )
	{
		jmlist_debug(__func__,"passing control to linked list seek_next routine.");
		return ijmlist_lnk_seek_next(jml,handle_ptr,ptr);
	}
#ifdef WITH_ASSOC_LIST
	else if( jml->flags & JMLIST_ASSOCIATIVE )
	{
		jmlist_debug(__func__,"passing control to the associative list seek_next routine.");
		return ijmlist_ass_seek_next(jml,handle_ptr,ptr);
	}
#endif
	
	jmlist_debug(__func__,"invalid or unsupported list type (jml=%p, flags=%u)",jml,jml->flags);
	jmlist_debug(__func__,"returning with failure.");
	jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
	return JMLIST_ERROR_FAILURE;
}

jmlist_status
ijmlist_idx_seek_next(jmlist jml,jmlist_seek_handle *handle_ptr,void **ptr)
{
	void *ptr_local;

	jmlist_debug(__func__,"called with jml=%p, handle_ptr=%p, ptr=%p",jml,handle_ptr,ptr);

	if( !jml ) {
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr ) {
		jmlist_debug(__func__,"invalid handle_ptr specified (handle_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !ptr ) {
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	bool fragmented;
	jmlist_is_fragmented(jml,false,&fragmented);
try_again:
	/* test next index bounds */
	if( (jml->flags & JMLIST_IDX_USE_SHIFT) || !fragmented )
	{
		if( handle_ptr->next_idx >= jml->idx_list.usage ) {
			jmlist_debug(__func__,"next index (%u) reached the limit of the list entry count (%u)",
					handle_ptr->next_idx,jml->idx_list.usage);
			jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
	} else
	{
		if( handle_ptr->next_idx >= jml->idx_list.capacity ) {
			jmlist_debug(__func__,"next index (%u) reached the limit of the list capacity (%u)",
					handle_ptr->next_idx,jml->idx_list.capacity);
			jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}
	}

	jmlist_debug(__func__,"accessing indexed list entry of index %u",handle_ptr->next_idx);
	ptr_local = jml->idx_list.plist[handle_ptr->next_idx];
	jmlist_debug(__func__,"indexed list entry of index %u has ptr=%p",handle_ptr->next_idx,ptr_local);

	if(	ptr_local == JMLIST_EMPTY_PTR ) {
		handle_ptr->next_idx++;
		jmlist_debug(__func__,"entry is empty, increasing to the next index %u",handle_ptr->next_idx);
		goto try_again;
	}

	jmlist_debug(__func__,"storing entry ptr=%p into ptr argument (ptr=%p)",ptr_local,ptr);
	*ptr = ptr_local;

	/* increase next index */
	handle_ptr->next_idx++;
	jmlist_debug(__func__,"increased handle (with ptr=%p) to the next index %u",
			handle_ptr,handle_ptr->next_idx);

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
	goto try_again;
}

jmlist_status
ijmlist_lnk_seek_next(jmlist jml,jmlist_seek_handle *handle_ptr,void **ptr)
{
	void *ptr_local;

	jmlist_debug(__func__,"called with jml=%p, handle_ptr=%p and ptr=%p",jml,handle_ptr,ptr);

	if( !jml ) {
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr ) {
		jmlist_debug(__func__,"invalid handle_ptr specified (handle_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !ptr ) {
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr->next_lnk ) {
		jmlist_debug(__func__,"there are no more entries to seek (reached the tail of the list)");
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	jmlist_debug(__func__,"accessing handle_ptr next link structure");
	ptr_local = handle_ptr->next_lnk->ptr;
	jmlist_debug(__func__,"storing entry ptr=%p into ptr argument (ptr=%p)",ptr_local,ptr);
	*ptr = ptr_local;

	handle_ptr->next_lnk = handle_ptr->next_lnk->next;
	jmlist_debug(__func__,"updated handle to the next entry (new next_lnk=%p)",handle_ptr->next_lnk);

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

#ifdef WITH_ASSOC_LIST
jmlist_status
ijmlist_ass_seek_next(jmlist jml,jmlist_seek_handle *handle_ptr,void **ptr)
{
	void *ptr_local;

	jmlist_debug(__func__,"called with jml=%p, handle_ptr=%p and ptr=%p",jml,handle_ptr,ptr);

	if( !jml ) {
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr ) {
		jmlist_debug(__func__,"invalid handle_ptr specified (handle_ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !ptr ) {
		jmlist_debug(__func__,"invalid ptr specified (ptr=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !handle_ptr->next_ass ) {
		jmlist_debug(__func__,"there are no more entries to seek (reached the tail of the list)");
		jmlist_errno = JMLIST_ERROR_OUT_OF_BOUNDS;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	jmlist_debug(__func__,"accessing handle_ptr next link structure");
	ptr_local = handle_ptr->next_ass->ptr;
	jmlist_debug(__func__,"storing entry ptr=%p into ptr argument (ptr=%p)",ptr_local,ptr);
	*ptr = ptr_local;

	handle_ptr->next_ass = handle_ptr->next_ass->next;
	jmlist_debug(__func__,"updated handle to the next entry (new next_ass=%p)",handle_ptr->next_ass);

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}
#endif

/*
   jmlist_find

   Parse all entries from the list, calling the callback function for each of them,
   when this callback returns result=jmlist_entry_found the seeking stops and this
   function returns the corresponding entry ptr in ptr argument.
*/
jmlist_status
jmlist_find(jmlist jml,JMLISTFINDCALLBACK callback,void *param,jmlist_lookup_result *result,void **ptr)
{
	jmlist_lookup_result result_local;
	jmlist_status jmls;
	jmlist_index entry_count;
	jmlist_seek_handle shandle;
	void *ptr_seek;

	jmlist_debug(__func__,"called with jml=%p, callback=%p, param=%p, result=%p and ptr=%p",
			jml,callback,param,result,ptr);

	if( !jml ) {
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !callback ) {
		jmlist_debug(__func__,"invalid callback specified (callback=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !result ) {
		jmlist_debug(__func__,"invalid result pointer specified (result=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	/* start seeking the list */

	jmls = jmlist_entry_count(jml,&entry_count);
	if( jmls != JMLIST_ERROR_SUCCESS ) {
		jmlist_debug(__func__,"unable to get entry count from list");
		jmlist_errno = JMLIST_ERROR_FAILURE;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	jmlist_debug(__func__,"seeking throughtout %u entries of the list",entry_count);

	jmls = jmlist_seek_start(jml,&shandle);
	if( jmls == JMLIST_ERROR_FAILURE ) {
		jmlist_debug(__func__,"unable to seek the list");
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	while(entry_count)
	{
		jmls = jmlist_seek_next(jml,&shandle,&ptr_seek);
		if( jmls != JMLIST_ERROR_SUCCESS ) {
			jmlist_debug(__func__,"seek_next returned failure, unexpected list seeking failure");
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}

		jmlist_debug(__func__,"got entry with ptr=%p, calling callback=%p",ptr_seek,callback);
		callback(ptr_seek,param,&result_local);
		jmlist_debug(__func__,"callback returned with result=%d",result_local);

		if( result_local == jmlist_entry_found )
		{
			jmlist_debug(__func__,"finished search, entry was found");
			if( ptr ) {
				jmlist_debug(__func__,"writing ptr=%p into ptr argument",ptr_seek);
				*ptr = ptr_seek;
			}

			jmlist_debug(__func__,"updating result argument");
			*result = result_local;

			jmls = jmlist_seek_end(jml,&shandle);
			if( jmls != JMLIST_ERROR_SUCCESS ) {
				jmlist_debug(__func__,"unable to end seek, jmlist_seek_end failed");
				jmlist_debug(__func__,"returning with failure.");
				return JMLIST_ERROR_FAILURE;
			}
			
			jmlist_debug(__func__,"returning with success.");
			return JMLIST_ERROR_SUCCESS;
		}
		entry_count--;
	}

	jmlist_debug(__func__,"seeking was ended, calling seek_end");
	jmls = jmlist_seek_end(jml,&shandle);
	if( jmls != JMLIST_ERROR_SUCCESS ) {
		jmlist_debug(__func__,"unable to end seek, jmlist_seek_end failed");
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	jmlist_debug(__func__,"entry was not found, updating result argument");
	*result = jmlist_entry_not_found;

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}

/*
   jmlist_parse

   This function will parse all the jmlist entries and call the callback
   for each entry. Its possible to pass a specific param to the callback
   also.
*/
jmlist_status
jmlist_parse(jmlist jml,JMLISTPARSERCALLBACK callback,void *param)
{
	jmlist_status jmls;
	jmlist_index entry_count;
	jmlist_seek_handle shandle;
	void *ptr_seek;

	jmlist_debug(__func__,"called with jml=%p, callback=%p, param=%p",
			jml,callback,param);

	if( !jml ) {
		jmlist_debug(__func__,"invalid jml specified (jml=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	if( !callback ) {
		jmlist_debug(__func__,"invalid callback specified (callback=0)");
		jmlist_errno = JMLIST_ERROR_INVALID_ARGUMENT;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	/* start seeking the list */

	jmls = jmlist_entry_count(jml,&entry_count);
	if( jmls != JMLIST_ERROR_SUCCESS ) {
		jmlist_debug(__func__,"unable to get entry count from list");
		jmlist_errno = JMLIST_ERROR_FAILURE;
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	jmlist_debug(__func__,"seeking throughtout %u entries of the list",entry_count);

	jmls = jmlist_seek_start(jml,&shandle);
	if( jmls == JMLIST_ERROR_FAILURE ) {
		jmlist_debug(__func__,"unable to seek the list");
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	while(entry_count)
	{
		jmls = jmlist_seek_next(jml,&shandle,&ptr_seek);
		if( jmls != JMLIST_ERROR_SUCCESS ) {
			jmlist_debug(__func__,"seek_next returned failure, unexpected list seeking failure");
			jmlist_debug(__func__,"returning with failure.");
			return JMLIST_ERROR_FAILURE;
		}

		jmlist_debug(__func__,"got entry with ptr=%p, calling callback=%p",ptr_seek,callback);
		callback(ptr_seek,param);
		jmlist_debug(__func__,"callback returned");

		entry_count--;
	}

	jmlist_debug(__func__,"seeking was ended, calling seek_end");
	jmls = jmlist_seek_end(jml,&shandle);
	if( jmls != JMLIST_ERROR_SUCCESS ) {
		jmlist_debug(__func__,"unable to end seek, jmlist_seek_end failed");
		jmlist_debug(__func__,"returning with failure.");
		return JMLIST_ERROR_FAILURE;
	}

	jmlist_debug(__func__,"returning with success.");
	return JMLIST_ERROR_SUCCESS;
}
