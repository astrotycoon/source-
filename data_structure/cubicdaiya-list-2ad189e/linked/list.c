/**
   In short, list.c is distributed under so called "BSD license",
   
   Copyright (c) 2009-2010 Tatsuhiko Kubo <cubicdaiya@gmail.com>
   All rights reserved.
   
   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:
   
   * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
   
   * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
   
   * Neither the name of the authors nor the names of its contributors
   may be used to endorse or promote products derived from this software 
   without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* written by C99 style */

#include "list.h"

/**
 * private function
 */
static list_t *list_extend(list_t *l);

/**
 * initialize list
 */
list_t *list_create(size_t esiz) {
    list_t *l;
    LIST_MALLOC(l, sizeof(list_t));
    l->head  = NULL;
    l->next  = NULL;
    l->esiz  = esiz;
    LIST_MALLOC(l->entry, l->esiz);
    return l;
}

/**
 * add an entryent to the tail of list
 */
list_t *list_add(list_t *l, list_entry_t *e) {
    list_t *ll = l;
    if (ll->head == NULL) {
        memcpy(ll->entry, e, ll->esiz);
        ll->head  = ll;
        return ll;
    }
    ll = list_extend(l);
    memcpy(ll->entry, e, ll->esiz);
    return ll;
}

/**
 * add an entryent to the head of list
 */
list_t *list_addfront(list_t *l, list_entry_t *e) {
    if (l->head == NULL) {
        memcpy(l->entry, e, l->esiz);
        l->head  = l;
        return l;
    }
    list_t *ln    = list_create(l->esiz);
    memcpy(ln->entry, e, l->esiz);
    ln->head      = ln;
    ln->next      = l->head->head;
    l->head->head = ln;
    return l;
}

/**
 * get the idxth entryent of list 
 */
list_t *list_get(list_t *l, uint_t idx) {
    if (idx < 0) {
        return NULL;
    }
    list_t *ll = list_head(l);
    for (int i=0;i<idx;++i) {
        if (ll->next == NULL) {
            return NULL;
        }
        ll = ll->next;
    }
    return ll;
}

/**
 * release all entries of list
 */
void list_destroy(list_t *l) {
    for (list_t *p=list_head(l);p!=NULL;) {
        list_t *current = p;
        list_t *next    = p->next;
        LIST_FREE(current->entry);
        LIST_FREE(current);
        p = next;    
    }
}

list_t *list_join(list_t *l1, list_t *l2) {
    l1->next = list_head(l2);
    l2->head = list_head(l1);
    return l2;
}

list_t *list_head(list_t *l) {
    return l->head->head;
}

list_t *list_insert(list_t *l, uint_t idx, list_entry_t *e) {
    if (idx == 0) {
        return list_addfront(l, e);
    }
    list_t *insert_prev  = list_get(l, idx - 1);
    if (insert_prev == NULL) {
        return l;
    }
    list_t *insert_after = insert_prev->next;
    if (insert_after == NULL) {
        return list_add(l, e);
    }
    list_t *insert    = list_create(l->esiz);
    memcpy(insert->entry, e, insert->esiz);
    insert->next      = insert_after;
    insert_prev->next = insert;
    return l;
}

list_t *list_remove(list_t *l, uint_t idx) {
    uint_t idx_prev = idx - 1;
    list_t *head = list_head(l);
    if (idx == 0) {
        if (head == NULL) {
            return NULL;
        }
        list_t *new_head = head->next;
        new_head->head = new_head;
        l->head = new_head;
        LIST_FREE(head->entry);
        LIST_FREE(head);
        return l;
    }
    list_t *remove_prev = list_get(l, idx_prev);
    list_t *remove;
    list_t *remove_after;
    if (remove_prev == NULL) {
        return l;
    }
    remove = remove_prev->next;
    if (remove) {
        remove_after = remove->next;
        LIST_FREE(remove->entry);
        LIST_FREE(remove);
        if (remove_after) {
            remove_prev->next  = remove_after;
            remove_after->head = head;
            return remove_after;
        } else {
            remove_prev->next = NULL;
            remove_prev->head = head;
            return remove_prev;
        }
    }
    return l;
}

bool list_entry_eq(list_t *l, list_entry_t *e) {
    return memcmp(l->entry, e, l->esiz) == 0;
}

/* following is private function */ 

/**
 * extend list
 */
static list_t *list_extend(list_t *l) {
    l->next = list_create(l->esiz);
    l->next->head = l->head->head;
    return l->next;
}
