
/* written by C99 style */

#include "mlist.h"

static mlist_t *mlist_extend(mlist_t *p);
static mlist_t *mlist_extend(mlist_t *p) {
  p->next        = mlist_create();
  p->next->begin = p->begin;
  return p->next;
}

mlist_t *mlist_create (void) {
  mlist_t *p;
  MLIST_MALLOC(p, sizeof(mlist_t));
  p->datum = NULL;
  p->next  = NULL;
  p->begin = p;
  return p;
}

void *mlist_alloc(mlist_t **p, size_t size) {
  mlist_t *lp = *p;
  MLIST_MALLOC(lp->datum, size);
  *p = mlist_extend(*p);
  return lp->datum;
}

void mlist_destroy (mlist_t *list) {
  for (mlist_t *p=list->begin;p!=NULL;) {
    mlist_t *current = p;
    mlist_t *next    = p->next;
    MLIST_FREE(current->datum);
    MLIST_FREE(current);
    p = next;
  }
}
