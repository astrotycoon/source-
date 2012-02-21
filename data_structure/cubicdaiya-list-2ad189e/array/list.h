

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIST_ARRAY_SIZE 1024

#define LIST_MALLOC(p, n)                       \
    do {                                        \
        if (((p) = malloc(n)) == NULL) {        \
            printf("malloc failed");            \
            exit(-1);                           \
        }                                       \
    } while(false)
#define LIST_FREE(p)                            \
    do {                                        \
        free(p);                                \
        (p) = NULL;                             \
    } while(false)

#define ACTUAL_IDX(virt_idx, list_id) ((virt_idx) - (list_id) * LIST_ARRAY_SIZE)

typedef int list_elem_t;
typedef unsigned int uint;

typedef struct list_t {
    uint id;
    list_elem_t datum[LIST_ARRAY_SIZE];
    uint idx;
    uint last_idx;
    struct list_t *head;
    struct list_t *next;
} list_t;

list_t *list_create(uint id);
list_elem_t list_get(list_t *l, uint idx);
list_t *list_add(list_t *l, list_elem_t d);
void   list_destroy(list_t *l);
list_elem_t list_iter(list_t **l, uint idx);

#endif
