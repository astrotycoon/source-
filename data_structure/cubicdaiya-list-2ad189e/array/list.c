#include "list.h"

static list_t *list_extend(list_t *l) {
    l->next = list_create(l->id + 1);
    l->next->head = l->head;
    return l->next;
}

list_t *list_create(uint id) {
    list_t *l;
    LIST_MALLOC(l, sizeof(list_t));
    l->id    = id;
    l->idx   = id * LIST_ARRAY_SIZE;
    l->last_idx = l->idx;
    l->head = l;
    l->next  = NULL;
    for (uint i=0;i<LIST_ARRAY_SIZE;++i) {
        l->datum[i] = 0;
    }
    return l;
}

list_t *list_add(list_t *l, list_elem_t d) {
    list_t *ll = NULL;
    uint idx = ACTUAL_IDX(l->idx, l->id);
    if (idx >= LIST_ARRAY_SIZE) {
        ll = list_extend(l);
        ll->datum[ACTUAL_IDX(ll->idx, ll->id)] = d;
        ll->idx++;
    } else {
        l->datum[ACTUAL_IDX(l->idx, l->id)]    = d;
        l->idx++;
        ll = l;
    }
    ll->last_idx++;
    return ll;
}

list_elem_t list_get(list_t *l, uint idx) {
    uint id = idx / LIST_ARRAY_SIZE;
    list_t *ll = l->head;
    for (int i=0;i<id;++i) {
        ll = ll->next;
    }
    if (ll == NULL) {
        return (list_elem_t)NULL;
    }
    return ll->datum[ACTUAL_IDX(idx, id)];
}

void list_destroy(list_t *l) {
    for (list_t *p=l->head;p!=NULL;) {
        list_t *current = p;
        list_t *next    = p->next;
        LIST_FREE(current);
        p = next;    
    }
}

list_elem_t list_iter(list_t **l, uint idx) {
    list_t *ll = *l;
    list_elem_t d;
    uint aidx = ACTUAL_IDX(idx, ll->id);
    if (aidx >= LIST_ARRAY_SIZE) {
        ll = ll->next;
        d  = ll->datum[ACTUAL_IDX(idx, ll->id)];
        *l = ll;
    } else {
        d = ll->datum[ACTUAL_IDX(idx, ll->id)];
    }
    return d;
}



