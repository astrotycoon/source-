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

#ifndef _JMLIST_H
#define _JMLIST_H

#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>

#define JMLIST_IDXLIST_DEF_MALLOC_INC 64
#define JMLIST_EMPTY_PTR (void*)(-1)

typedef uint32_t jmlist_index;
typedef void (*JMLISTPARSERCALLBACK) (void *ptr,void *param);

typedef enum _jmlist_status
{
	JMLIST_ERROR_SUCCESS = 0,
	JMLIST_ERROR_FAILURE = 1,
	JMLIST_ERROR_UNIMPLEMENTED = 2,
	JMLIST_ERROR_MALLOC = 3,
	JMLIST_ERROR_INVALID_ARGUMENT,
	JMLIST_ERROR_OUT_OF_BOUNDS,
	JMLIST_ERROR_DAMAGED_LIST,
	JMLIST_ERROR_EMPTY_LIST,
	JMLIST_ERROR_ENTRY_NOT_FOUND,
	JMLIST_ERROR_UNSUPPORTED
} jmlist_status;

/* flags of jmlist engine */
typedef enum _jmlist_init_flags
{
	JMLIST_FLAG_INTERNAL_LIST = 1,
	JMLIST_FLAG_MEM_COUNTER = 2,
	JMLIST_FLAG_VERBOSE = 4,
	JMLIST_FLAG_DEBUG = 8
} jmlist_init_flags;
#define JMLIST_INIT_FLAGS 2

/* flags of jmlist's
 * 
 * JMLIST_IDX_USE_SHIFT
 * shifts entries when an entry from the middle of the list is removed.
 * this avoids the list being fragmented with the cost of O(N)
 * for each jmlist_remove_* applied.
 *
 * JMLIST_LNK_INSERT_AT_TAIL
 * when jmlist_insert is used on a linked list, the new entry is inserted
 * in the tail of the list. operation (insert*) costs O(N) in worst case.
 *
 * JMLIST_IDX_USE_FRAG_FLAG
 * some operations on the list require that the list isn't fragmented, jmlist
 * has a function to check if the list is fragmented, this function can work
 * in two ways: checking for frag flag or parsing the whole list a look for
 * holes, so basicly a difference from O(k) to O(N). the code of O(N) is more
 * reliable than O(k), because in the use of frag flag all holes must be
 * detected with conditionals (the programmer must know all the possible
 * cases where holes can be caused).
 * 
 */
typedef enum _jmlist_flags
{
	JMLIST_LINKED = 1,
	JMLIST_INDEXED = 2,
	JMLIST_IDX_USE_SHIFT = 4,
	JMLIST_LNK_INSERT_AT_TAIL = 8,
	JMLIST_ASS_INSERT_AT_TAIL = 8,
	JMLIST_IDX_USE_FRAG_FLAG = 16,
	JMLIST_LNK_INTERNAL = 32,
	JMLIST_ASSOCIATIVE = 64
} jmlist_flags;
#define JMLIST_FLAGS 2

typedef enum _jmlist_lookup_result
{
	jmlist_entry_not_found,
	jmlist_entry_found
} jmlist_lookup_result;

typedef jmlist_status (*JMLISTFINDCALLBACK) (void *ptr,void *param,jmlist_lookup_result *result);
typedef jmlist_status (*JMLISTDUMPCALLBACK) (void *ptr,int options);

typedef struct _jmlist_params
{
	struct {
		jmlist_index malloc_inc;
	} idx_list;
	struct {
		jmlist_index init_size;
	} lnk_list;
	char tag[16];
	jmlist_flags flags;
} *jmlist_params;

typedef struct _jmlist_init_params {
	jmlist_init_flags flags;
	FILE *fverbose,*fdump,*fdebug;
} *jmlist_init_params;

typedef struct _linked_entry
{
		struct _linked_entry *next;
		void *ptr;
} linked_entry;

typedef void* jmlist_key;
typedef uint32_t jmlist_key_length;
#ifdef WITH_ASSOC_LIST

typedef struct _assoc_entry
{
		struct _assoc_entry *next;
		jmlist_key key_ptr;
		jmlist_key_length key_len;
		void *ptr;
} assoc_entry;
#endif

typedef struct _jmlist
{
	jmlist_flags flags;
	bool seeking;
	struct {
		void **plist;
		jmlist_index capacity;
		jmlist_index usage;
		jmlist_index malloc_inc;
		bool fragmented;
	} idx_list;
	struct {
		linked_entry *phead;
		jmlist_index usage;
	} lnk_list;
#ifdef WITH_ASSOC_LIST
	struct {
		assoc_entry *phead;
		jmlist_index usage;
	} ass_list;
#endif
	char tag[16];
} *jmlist;

typedef struct _dlinked_entry
{
	struct _dlinked_entry *next;
	struct _dlinked_entry *prev;
	void *ptr;
} dlinked_entry;

typedef struct _jmlist_memory_info
{
	struct {
		uint32_t total;
		uint32_t used;
	} idx_list;
	struct {
		uint32_t total;
		uint32_t used;
	} lnk_list;
	struct {
		uint32_t total;
		uint32_t used;
	} ass_list;
	uint32_t total;
	uint32_t used;
} *jmlist_memory_info;

typedef union _jmlist_seek_handle {
	jmlist_index next_idx;
	linked_entry *next_lnk;
#ifdef WITH_ASSOC_LIST
	assoc_entry *next_ass;
#endif
} jmlist_seek_handle;

#define DEBUGSTART if(jmlist_cfg.flags & JMLIST_FLAG_DEBUG) {
#define DEBUGEND }

#define VERBSTART if(jmlist_cfg.flags & JMLIST_FLAG_VERBOSE) {
#define VERBEND }

#define DCHECKSTART
#define DCHECKEND

#ifdef WITH_KEY_TREE
/*
 key tree is a way of finding an entry much faster. each key i
 has key_len_i bytes. the key tree consists of rows of bytes,
 lets start with the case of rows of 256 bytes. each byte is
 associated to a byte in the key. each row has a pointer to
 the next row. the row number is associated with the byte index
 of the key. lets see an example: key=(1,5,11)
 first row, get the byte index 1 */
#endif

jmlist_status jmlist_initialize(jmlist_init_params params);
jmlist_status jmlist_uninitialize();
jmlist_status jmlist_create(jmlist *new_jml,jmlist_params params);
jmlist_status jmlist_ptr_exists(jmlist jml,void *ptr,jmlist_lookup_result *result);
jmlist_status jmlist_get_by_index(jmlist jml,jmlist_index index,void **ptr);
jmlist_status jmlist_remove_by_ptr(jmlist jml,void *ptr);
jmlist_status jmlist_remove_by_index(jmlist jml,jmlist_index index);
jmlist_status jmlist_free(jmlist jml);
jmlist_status jmlist_push(jmlist jml,void *ptr);
jmlist_status jmlist_pop(jmlist jml,void **ptr);
jmlist_status jmlist_insert(jmlist jml,void *ptr);
jmlist_status jmlist_dump(jmlist jml);
jmlist_status jmlist_dump_pretty(jmlist jml,JMLISTDUMPCALLBACK callback,int options);
void jmlist_debug(const char *func,const char *fmt,...);
jmlist_status jmlist_status_to_string(jmlist_status status,char *output,size_t output_len);
jmlist_status jmlist_is_fragmented(jmlist jml,bool force_seeker,bool *fragmented);
jmlist_status jmlist_memory(jmlist_memory_info jml_mem);
jmlist_status jmlist_free_all(void);
jmlist_status jmlist_parse(jmlist jml,JMLISTPARSERCALLBACK callback,void *param);
jmlist_status jmlist_find(jmlist jml,JMLISTFINDCALLBACK callback,void *param,jmlist_lookup_result *result,void **ptr);
jmlist_status jmlist_seek_start(jmlist jml,jmlist_seek_handle *handle_ptr);
jmlist_status jmlist_seek_next(jmlist jml,jmlist_seek_handle *handle_ptr,void **ptr);
jmlist_status jmlist_seek_end(jmlist jml,jmlist_seek_handle *handle_ptr);
jmlist_status jmlist_entry_count(jmlist jml,jmlist_index *entry_count);
jmlist_status jmlist_remove_by_index(jmlist jml,jmlist_index index);
jmlist_status jmlist_replace_by_index(jmlist jml,jmlist_index index,void *new_ptr);

#ifdef WITH_ASSOC_LIST
jmlist_status jmlist_insert_with_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len,void *ptr);
jmlist_status jmlist_get_by_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len,void **ptr);
jmlist_status jmlist_remove_by_key(jmlist jml,jmlist_key key_ptr,jmlist_key_length key_len);
#endif

/* TODO

 jmlist_status jmlist_insert_at_position(jmlist jml,void *ptr,jmlist_position pos);
  where pos = JMLIST_HEAD, JMLIST_TAIL, JMLIST_FREE_HEAD, JMLIST_FREE_TAIL

 */
#endif
