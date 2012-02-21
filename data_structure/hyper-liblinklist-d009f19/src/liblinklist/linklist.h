/*

	liblinklist
	(c) Copyright Hyper-Active Systems, Australia

	Contact:
		Clinton Webb
		webb.clint@gmail.com

	This is a small, simplistic library designed to handle a linked list of generic objects.

	It is released under GPL v2 or later license.  Details are included in the LICENSE file.

*/

#ifndef __LINKLIST_H
#define __LINKLIST_H


#define LIBLINKLIST_VERSION 0x00009010
#define LIBLINKLIST_VERSION_TEXT "v0.90.10"


typedef struct __llist {
	void *data;
	struct __llist *prev, *next;
} _list_node_t;

typedef struct {
	_list_node_t *head;
	_list_node_t *tail;
	_list_node_t *pool;
	int items;
	char *join;
	_list_node_t *loop;
	short int internally_created;
} list_t;

// initializing and de-initializing of the list.
list_t * ll_init(list_t *list);
list_t * ll_free(list_t *list);

// adding data to the list.
void ll_push_head(list_t *list, void *data);
void ll_push_tail(list_t *list, void *data);

// getting and removing data from the list.
void * ll_pop_head(list_t *list);
void * ll_pop_tail(list_t *list);

// getting data from the list without removing.
void * ll_get_head(list_t *list);
void * ll_get_tail(list_t *list);

// Methods to iterate throught the list.
void ll_start(list_t *list);
void * ll_next(list_t *list);
void ll_remove(list_t *list, void *ptr);
void ll_finish(list_t *list);

// statistics of the linked list.
int ll_count(list_t *list);

// create a string of all the entries in the list, seperated by the 'sep' string.
char * ll_join_str(list_t *list, const char *sep);

// move the entry to the head or tail of a list.  Most likely this would be
// used while doing looping through the list.  Since the previous functionality
// of going through the list would have removed the entry and then added it, we
// will leave the 'next' pointer pointing to the next entry in the list before
// the move took place.  This means that if you move an entry to the tail of
// the list, it will likely get processed again if you continue the loop.
void ll_move_head(list_t *list, void *ptr);
void ll_move_tail(list_t *list, void *ptr);

#endif
