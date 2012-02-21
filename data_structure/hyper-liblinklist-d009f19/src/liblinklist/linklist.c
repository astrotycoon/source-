/*

	liblinklist
	(c) Copyright Hyper-Active Systems, Australia

	Contact:
		Clinton Webb
		webb.clint@gmail.com

*/


#include "linklist.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#if (LIBLINKLIST_VERSION != 0x00009010)
	#error "This version certified for v0.90.10 only"
#endif

//-----------------------------------------------------------------------------
// Initialise the list structure.  
list_t * ll_init(list_t *list)
{
	list_t *l;
	
	if (list) {
		l = list;
		l->internally_created = 0;
	}
	else {
		l = malloc(sizeof(*l));
		l->internally_created = 1;
	}
	
	l->head = NULL;
	l->tail = NULL;
	l->pool = NULL;
	l->items = 0;
	l->join = NULL;
	l->loop = NULL;
	
	return(l);
}

//-----------------------------------------------------------------------------
// Free the resources created by the list.  At this point there should not be
// any nodes in the list.  Will not free the actual list itself... as it could
// have been malloc'd, or it could be embedded in another structure, or an
// auto-var.
//
// if the list was was created internally, then this function will return a 
// NULL.  If the list was created externally, and passed in as a parameter to 
// ll_init() then it will return the pointer.  THis is useful for making sure 
// that things are free'd the same way they are created.  
//
//     somelist = ll_init(NULL);
//     ...
//     somelist = ll_free(somelist);
//     assert(somelist == NULL);
//
list_t * ll_free(list_t *list)
{
	_list_node_t *node, *tmp;
	assert(list);

	assert(list->head == NULL);
	assert(list->tail == NULL);
	assert(list->loop == NULL);

	node = list->pool;
	while (node) {
		assert(node->data == NULL);
		tmp = node;
		node = node->next;
		free(tmp);
	}
	list->pool = NULL;

	if (list->join) {
		free(list->join);
		list->join = NULL;
	}
	
	if (list->internally_created > 0) {
		free(list);
		return(NULL);
	}
	else {
		return(list);
	}
}

//-----------------------------------------------------------------------------
// Internal function to return a new node wrapper from the pool.  If there
// isn't any left in the pool, then create a new node wrapper, and return that
// instead.
static _list_node_t * new_node(list_t *list)
{
	_list_node_t *node;

	assert(list);
	
	if (list->pool) {
		// we have a node already available in the pool, so we will use that.
		node = list->pool;
		list->pool = node->next;
		node->next = NULL;
	}
	else {
		// There aren't anymore nodes in the pool, so we need to create one.
		node = calloc(1, sizeof(*node));
		assert(node);
		node->data = NULL;
		node->next = NULL;
		node->prev = NULL;
	}
	
	assert(node);
	assert(node->data == NULL);
	assert(node->prev == NULL);
	assert(node->next == NULL);
	return(node);
}

//-----------------------------------------------------------------------------
// Internal function to return a node-wrapper back to the pool.
static void return_node(list_t *list, _list_node_t *node)
{
	assert(list);
	assert(node);

	assert(node != list->tail);
	assert(node != list->head);

	assert(node->data == NULL);
	assert(node->next == NULL);
	assert(node->prev == NULL);

	node->next = list->pool;
	list->pool = node;
}

//-----------------------------------------------------------------------------
// push an object to the head of the list.  Will add the data pointer to a
// node-wrapper that contains all the actual list control mechanisms.
void ll_push_head(list_t *list, void *data)
{
	_list_node_t *node;
	
	assert(list);
	assert(data);

	node = new_node(list);
	node->data = data;
	assert(node->prev == NULL);
	node->next = list->head;
	if (node->next) node->next->prev = node;
	list->head = node;
	if (list->tail == NULL) list->tail = node;
	list->items ++;
}

//-----------------------------------------------------------------------------
// same as ll_push_head, except pushing to the bottom of the list instead.
void ll_push_tail(list_t *list, void *data)
{
	_list_node_t *node;
	
	assert(list);
	assert(data);

	node = new_node(list);
	node->data = data;
	assert(node->next == NULL);
	node->prev = list->tail;
	if (node->prev) node->prev->next = node;
	list->tail = node;
	if (list->head == NULL) list->head = node;
	list->items ++;
}


//-----------------------------------------------------------------------------
// delete a particular node from the list.
static void delete_node(list_t *list, _list_node_t *node)
{
	assert(list);
	assert(node);
	assert(list->head);
	assert(list->tail);

	if (list->loop == node) {
		list->loop = node->next;
	}

	assert(list->items > 0);
	list->items --;
	assert(list->items >= 0);

	if (node->prev) {
		assert(node->prev->next == node);
		node->prev->next = node->next;
	}
	if (node->next) {
		assert(node->next->prev == node);
		node->next->prev = node->prev;
	}
	
	if (list->head == node) list->head = node->next;
	if (list->tail == node) list->tail = node->prev;
	
	node->prev = NULL;
	node->next = NULL;
	node->data = NULL;
	return_node(list, node);
}




//-----------------------------------------------------------------------------
// return a pointer to the data object in the first entry in the list.  Does
// not remove the entry from the list.  This is useful for examining an item
// before determining to remove it.
void * ll_get_head(list_t *list)
{
	assert(list);

	if (list->head) {
		assert(list->head->data);
		return(list->head->data);
	}
	else {
		return(NULL);
	}
}

//-----------------------------------------------------------------------------
// same as ll_get_head except it gets the node from the tail.
void * ll_get_tail(list_t *list)
{
	assert(list);

	if (list->tail) {
		assert(list->tail->data);
		return(list->tail->data);
	}
	else {
		return(NULL);
	}
}

//-----------------------------------------------------------------------------
// Pop an entry from the top of the list.  Which means that it returns the data
// pointer if there is a node, and removes the node from the list.
void * ll_pop_head(list_t *list)
{
	void *data;
	
	assert(list);

	if (list->head) {
		assert(list->head->data);
		data = list->head->data;

		assert(list->items > 0);
		delete_node(list, list->head);
		assert(list->items >= 0);
	}
	else {
		assert(list->items == 0);
		data = NULL;
	}
	
	return(data);
}

//-----------------------------------------------------------------------------
// Same as ll_pop_head except pops from the bottom of the list.
void * ll_pop_tail(list_t *list)
{
	void *data;
	
	assert(list);

	if (list->tail) {
		assert(list->tail->data);
		data = list->tail->data;

		assert(list->items > 0);
		delete_node(list, list->tail);
		assert(list->items >= 0);
	}
	else {
		assert(list->items == 0);
		data = NULL;
	}
	
	return(data);
}


//-----------------------------------------------------------------------------
// Start the iterations through the list.  If there are no entries in the list,
// will return NULL.
void ll_start(list_t *list)
{
	assert(list);
	assert(list->loop == NULL);
	list->loop = list->head;
}

//-----------------------------------------------------------------------------
// return the data pointer for the next node.
void * ll_next(list_t *list)
{
	void *ptr;
	
	assert(list);

	if (list->loop == NULL) {
		return(NULL);
	}
	else {
		ptr = list->loop->data;
		assert(ptr);
		list->loop = list->loop->next;
		return(ptr);
	}
}

//-----------------------------------------------------------------------------
// This function is used to indicate that the iteration loop is finished.
// Technically it is not necessary, but it allows us to assist developers in
// finding obscure bugs in loops.  So if a loop is started, but not finished,
// and another loop is started, then we generate an assertion.
void ll_finish(list_t *list)
{
	assert(list);
	list->loop = NULL;
}

//-----------------------------------------------------------------------------
// Remove a particular pointer from the list.  The 'next' is used to give a
// hint to where in the list the item is (normally provided as part of
// ll_start/ll_next).  If next is NULL, then the tail of the list is checked
// first.
void ll_remove(list_t *list, void *ptr)
{
	_list_node_t *node;
	
	assert(list && ptr);

	assert(list->head);
	assert(list->tail);

	// first check the 'list->loop' hint.  If it is not NULL, then it should 
	// be either pointing to the one we want, or pointing to the next one in 
	// the list from it.
	if (list->loop) {
		assert(list->loop->data);
		if (list->loop->data == ptr) {
			delete_node(list, list->loop);
			return;
		}
		else if (list->loop->prev) {
			// the entry we are looking at wasnt the right one.  However, the 
			// previous one might be, so we will check that too.
			assert(list->loop->prev->data);
			if (list->loop->prev->data == ptr) {
				delete_node(list, list->loop->prev);
				return;
			}
		}
	}

	// we didn't have a hint 'loop' that told us where the entry was, so we need
	// to go through the list.  We will start at the tail.
	node = list->tail;
	while (node) {
		assert(node->data);
		if (node->data == ptr) {
			assert(list->items > 0);
			delete_node(list, node);
			assert(list->items >= 0);
			return;
		}
		
		assert(node->prev == NULL || (node->prev->next == node && node->prev != node));
		node = node->prev;
	}

	assert(list->items >= 0);
	assert((list->items == 0 && list->head == NULL && list->tail == NULL) || (list->items > 0 && list->head && list->tail));
	
	// if we get this far, then obviously we were asked to remove something that wasn't in the list.
	assert(0);
}


//-----------------------------------------------------------------------------
// Return the number of items in the list.
int ll_count(list_t *list)
{
	assert(list);
	assert(list->items >= 0);
	return(list->items);
}


//-----------------------------------------------------------------------------
// Assuming that the list contains valid strings, this function will join all
// the elements in the list into a single string
char * ll_join_str(list_t *list, const char *sep)
{
	int max;
	int count;
	char *str;
	
	assert(list);

	// first go thru the list, and calculate the size of the resulting string.
	max = 0;
	count = 0;
	ll_start(list);
	while ((str = ll_next(list))) {
		count ++;
		max += strlen(str);
	}
	ll_finish(list);

	if (sep && count > 1) {
		max += (strlen(sep) * count);
	}

	// max now contains the size of the string we will be building.
	list->join = (char *) realloc(list->join, max+1);
	assert(list->join);
	
	// now build the list.
	count = 0;
	list->join[0] = '\0';
	ll_start(list);
	while ((str = ll_next(list))) {

		// TODO: strcat is not very optimal way to do this.  We should keep track
		// of the end of the string and add it direct to the end, but will bother
		// with that another day.

		if (count > 0 && sep != NULL) {
			strcat(list->join, sep);
		}
		strcat(list->join, str);

		count++;
	}
	ll_finish(list);

// 	fprintf(stderr, "ll_join_str: len=%d, max=%d\n", strlen(list->join), max);
	assert(strlen(list->join) <= max);

	assert(list->join);
	return(list->join);
}


//-----------------------------------------------------------------------------
// move the entry to the head or tail of a list.  Most likely this would be
// used while doing looping through the list.  Since the previous functionality
// of going through the list would have removed the entry and then added it, we
// will leave the 'next' pointer pointing to the next entry in the list before
// the move took place.  This means that if you move an entry to the tail of
// the list, it will likely get processed again if you continue the loop.
void ll_move_head(list_t *list, void *ptr)
{
	_list_node_t *node;
	assert(list);
	assert(ptr);
	assert(list->items > 0);

	// start off with node pointing to NULL.
	node = NULL;

	// first we check to see if the 'loop' points us to the node we are looking
	// for.  Either at the loop entry itself, or the previous one.
	if (list->loop) {
		assert(list->loop->data);
		if (list->loop->data == ptr) {
			node = list->loop;
		}
		else if (list->loop->prev) {
			assert(list->loop->prev->data);
			if (list->loop->prev->data == ptr) {
				node = list->loop->prev;
			}
		}
	}

	// If we haven't already found the node we want, we need to go through the
	// list.  We will start at the tail.
	if (node == NULL) node = list->tail;

	// at this point, we should definately have a node to look at.  We dont
	// know if it is the correct one, but it is either the tail, or the one we
	// are looking for... or possibly both.
	assert(node);

	// If we already found the node, then we will go right into this while loop,
	// and hit straight away... otherwise we will loop through.  Code path is a
	// little unclear, but it means that the actual guts of the thing is in one
	// location.
	while (node) {
		assert(node->data);
		if (node->data == ptr) {

			// if the node we are looking for is already at the head of the list,
			// we can effectively do nothing.
			if (node != list->head) {

				// if the node we are looking for is also listed as the next one in a
				// loop, then we should move the 'loop' pointer to the next node in
				// the list.
				if (list->loop == node) {
					list->loop = node->next;
				}

				// mvoe the pointers around so that the node is removed from where it
				// is, and placed at the head of the list.
				if (node->prev) node->prev->next = node->next;
				if (node->next) node->next->prev = node->prev;
				if (list->tail == node) list->tail = node->prev;
				node->prev = NULL;
				node->next = list->head;
				if (node->next) node->next->prev = node;
				list->head = node;
				assert(list->tail);
				assert(list->head);
				assert(list->tail != list->head);
			}
			node = NULL;		
		}
		else {
			// we haven't yet found the node we are looking for, so we need to keep
			// going up the list.
			node = node->prev;
		}
	}
}

void ll_move_tail(list_t *list, void *ptr)
{
	_list_node_t *node;
	assert(list);
	assert(ptr);
	assert(list->items > 0);

	// start off with node pointing to NULL.
	node = NULL;

	// first we check to see if the 'loop' points us to the node we are looking
	// for.  Either at the loop entry itself, or the previous one.
	if (list->loop) {
		assert(list->loop->data);
		if (list->loop->data == ptr) {
			node = list->loop;
		}
		else if (list->loop->prev) {
			assert(list->loop->prev->data);
			if (list->loop->prev->data == ptr) {
				node = list->loop->prev;
			}
		}
	}

	// If we haven't already found the node we want, we need to go through the
	// list.  We will start at the tail.
	if (node == NULL) node = list->tail;

	// at this point, we should definately have a node to look at.  We dont
	// know if it is the correct one, but it is either the tail, or the one we
	// are looking for... or possibly both.
	assert(node);

	// If we already found the node, then we will go right into this while loop,
	// and hit straight away... otherwise we will loop through.  Code path is a
	// little unclear, but it means that the actual guts of the thing is in one
	// location.
	while (node) {
		assert(node->data);
		if (node->data == ptr) {

			// if the node we are looking for is already at the head of the list,
			// we can effectively do nothing.
			if (node != list->tail) {

				// if the node we are looking for is also listed as the next one in a
				// loop, then we should move the 'loop' pointer to the next node in
				// the list.
				if (list->loop == node) {
					list->loop = node->next;
				}

				// move the pointers around so that the node is removed from where it
				// is, and placed at the tail of the list.
				if (node->prev) node->prev->next = node->next;
				if (node->next) node->next->prev = node->prev;
				if (list->head == node) list->head = node->next;
				node->next = NULL;
				node->prev = list->tail;
				if (node->prev) node->prev->next = node;
				list->tail = node;
				assert(list->tail);
				assert(list->head);
				assert(list->tail != list->head);
			}
			node = NULL;		
		}
		else {
			// we haven't yet found the node we are looking for, so we need to keep
			// going up the list.
			node = node->prev;
		}
	}
}

