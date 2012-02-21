/**
   In short, list.h is distributed under so called "BSD license",
   
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

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LIST_MALLOC(p, n)                       \
    do {                                        \
        if (((p) = malloc(n)) == NULL) {        \
            fprintf(stderr, "malloc failed");   \
            exit(-1);                           \
        }                                       \
    } while(false)
#define LIST_FREE(p)                            \
    do {                                        \
        free(p);                                \
        (p) = NULL;                             \
    } while(false)

typedef void list_entry_t;
typedef unsigned int uint_t;

typedef struct list_t {
    list_entry_t   *entry; // entryent
    size_t         esiz; // entryent size
    struct list_t *head; // pointer to head of list
    struct list_t *next; // pointer to next entryent
} list_t;

list_t *list_create(size_t esiz);
list_t *list_get(list_t *l, uint_t idx);
list_t *list_add(list_t *l, list_entry_t *e);
list_t *list_addfront(list_t *l, list_entry_t *e);
list_t *list_join(list_t *l1, list_t *l2);
list_t *list_head(list_t *l);
list_t *list_insert(list_t *l, uint_t idx, list_entry_t *e);
list_t *list_remove(list_t *l, uint_t idx);
bool list_entry_eq(list_t *l, list_entry_t *e);
void list_destroy(list_t *l);

#endif
