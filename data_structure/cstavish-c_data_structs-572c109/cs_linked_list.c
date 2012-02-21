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

#include "cs_linked_list.h"
#include <stdarg.h>
#include <stdlib.h>

static cs_sll_node *get_sll_(cs_sll *list, size_t index) {
    if (list == NULL)
        return NULL;
    
    cs_sll_node *n = list->start;
    for (int i = 0; n != NULL; i++, n = n->next) {
        if (i == index)
            return n;
    }
    return NULL;
}

static cs_dll_node *get_dll_(cs_dll *list, size_t index) {
    if (list == NULL)
        return NULL;
    
    // take advantage of the doubly-linked nature of the list
    // - choose the shortest route from either end to index
    if (index > list->size / 2) {
        index = list->size - index - 1;
        cs_dll_node *n = list->end;
        for (int i = 0; n != NULL; i++, n = n->prev) {
            if (i == index)
                return n;
        }   
    }
    else {
        cs_dll_node *n = list->start;
        for (int i = 0; n != NULL; i++, n = n->next) {
            if (i == index)
                return n;
        }
    }
    return NULL;
}

static cs_sll_node *destroy_node_sll_(cs_sll_node *n, void(*cleanup)(void *)) {
    if (n == NULL)
        return NULL;

    if (cleanup)
        cleanup(n->data);
    
    cs_sll_node *next = n->next;
    free(n);
    return next;
}

static cs_dll_node *destroy_node_dll_(cs_dll_node *n, void(*cleanup)(void *)) {
    if (n == NULL)
        return NULL;

    if (cleanup)
        cleanup(n->data);
        
    cs_dll_node *next = n->next;
    free(n);
    return next;
}

void *cs_sll_get(cs_sll *list, size_t index) {
    cs_sll_node *n = get_sll_(list, index);
    if (n == NULL)
        return NULL;
    return n->data;
}

uint8_t cs_sll_ins(cs_sll *list, void *data, size_t index) {
    if (list == NULL)
        return 0;
    
    cs_sll_node *new = malloc(sizeof(cs_sll_node));
    if (new == NULL)
        return 0;

    if (index == 0) {
        new->next = list->start;
        list->start = new;
    }
    else {
        cs_sll_node *p = get_sll_(list, index - 1);
        new->next = p->next;
        p->next = new;
    }
    
    list->size++;
    new->data = data;
    return 1;
}

void *cs_sll_del(cs_sll *list, size_t index) {
    if (list == NULL)
        return NULL;
    
    if (index >= list->size)
        return NULL;

    cs_sll_node *n = NULL;

    // remove first element and 'pull the chain down'
    if (index == 0) {
        n = list->start;
        list->start = list->start->next;
    }
    // remove any other element
    else {
        cs_sll_node *prev = get_sll_(list, index - 1);
        n = prev->next;
        prev->next = n->next;
    }

    list->size--;

    void *data = n->data;
    free(n);
    return data;
}

uint8_t cs_sll_app(cs_sll *list, void *data) {
    size_t index = list->size;
    return cs_sll_ins(list, data, index);
}

void *cs_dll_get(cs_dll *list, size_t index) {
    cs_dll_node *n = get_dll_(list, index);
    if (n == NULL)
        return NULL;
    return n->data;
}

uint8_t cs_dll_ins(cs_dll *list, void *data, size_t index) {
    if (list == NULL)
        return 0;
    
    cs_dll_node *new = malloc(sizeof(cs_dll_node));
    if (new == NULL)
        return 0;

    if (index == 0) {
        new->next = list->start;
        new->prev = NULL;
        list->start = new;
        if (new->next)
            new->next->prev = new;
        if (list->end == NULL)
            list->end = new;
    }
    else {
        cs_dll_node *p = get_dll_(list, index - 1);
        new->next = p->next;
        
        // interior insertion
        if (new->next)
            new->next->prev = new;
        // append
        else
            list->end = new;
        
        p->next = new;
        new->prev = p;
    }
    
    list->size++;
    new->data = data;
    
    return 1;
}

uint8_t cs_sll_swap(cs_sll *list, size_t x, size_t y) {
    if (list == NULL || list->size < x || list->size < y)
        return 0;
        
    if (x == y)
        return 1;
    
    cs_sll_node *a = get_sll_(list, x), *b = get_sll_(list, y);
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
    
    return 1;
}

uint8_t cs_sll_set(cs_sll *list, void *data, size_t index) {
    if (list == NULL || list->size < index)
        return 0;
        
    get_sll_(list, index)->data = data;
    
    return 1;
}

void *cs_dll_del(cs_dll *list, size_t index) {
    if (list == NULL)
        return NULL;
    
    if (index >= list->size)
        return NULL;

    cs_dll_node *n = get_dll_(list, index);
    if (n == NULL)
        return NULL;
    
    // removing first node
    if (n->prev == NULL) {
        if (n == list->end)
            list->end = NULL;
        list->start = n->next;
    }
    // removing last node
    else if (n->next == NULL) {
        list->end = n->prev;
        n->prev->next = NULL;
    }
    // removing interior node
    else {
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }
    
    void *data = n->data;
    free(n);
    return data;
}

uint8_t cs_dll_set(cs_dll *list, void *data, size_t index) {
    if (list == NULL || list->size < index)
        return 0;
        
    get_dll_(list, index)->data = data;
    
    return 1;
}

uint8_t cs_dll_swap(cs_dll *list, size_t x, size_t y) {
    if (list == NULL || list->size < x || list->size < y)
        return 0;
        
    if (x == y)
        return 1;
    
    cs_dll_node *a = get_dll_(list, x), *b = get_dll_(list, y);
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
    
    return 1;
}


uint8_t cs_dll_app(cs_dll *list, void *data) {
    return cs_dll_ins(list, data, list->size);
}

cs_sll *cs_sll_create(void (*cleanup)(void *), ...)  {
    cs_sll *list = malloc(sizeof(cs_sll));
    if (list == NULL)
        return NULL;

    list->size = 0;
    list->cleanup = cleanup;
    list->start = NULL;
    
    va_list v;
    va_start(v, cleanup);
    void *data = NULL;
    while ((data = va_arg(v, void *))) {
        cs_sll_app(list, data);
    }
    va_end(v);
    
    return list;
}

void cs_sll_destroy(cs_sll *list) {
    if (list == NULL)
        return;
    
    // destroy each node, then free the list head
    cs_sll_node *n = list->start;
    while ((n = destroy_node_sll_(n, list->cleanup)))
        ;
    free(list);
}

cs_dll *cs_dll_create(void (*cleanup)(void *), ...) {
    cs_dll *list = malloc(sizeof(cs_dll));
    if (list == NULL)
        return NULL;
    
    list->size = 0;
    list->cleanup = cleanup;
    list->start = list->end = NULL;
    
    va_list v;
    va_start(v, cleanup);
    void *data = NULL;
    while ((data = va_arg(v, void *))) {
        cs_dll_app(list, data);
    }
    va_end(v);
    
    return list;
}

void cs_dll_destroy(cs_dll *list) {
    if (list == NULL)
        return;
    
    cs_dll_node *n = list->start;
    while (n && (n = destroy_node_dll_(n, list->cleanup)))
        ;
    free(list);
}