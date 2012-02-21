#include "skiplist.h"
#include <assert.h>
#include <stdlib.h>

#define HEAD_MAGIC ((void *) 1)
#define SL_IS_HEAD(sl) ((sl)->val_ == HEAD_MAGIC)

static void sl_init_head(struct skiplist *sl) {
    sl->next = sl;
    sl->dup = NULL;
    sl->val_ = HEAD_MAGIC;
}

static struct skiplist *sl_new_head(void) {
    struct skiplist *new_ = (struct skiplist *) malloc(sizeof(struct skiplist));
    sl_init_head(new_);
    return new_;
}

void sl_init(SkipList sl, sl_cmp_fn cmp, void *data) {
    sl->top = sl->bottom = sl_new_head();
    sl->cmp_ = cmp;
    sl->data_ = data;
}

SkipList sl_new(sl_cmp_fn cmp, void *data) {
    SkipList sl = (SkipList) malloc(sizeof(*sl));
    sl_init(sl, cmp, data);
    return sl;
}

static struct skiplist *sl_insert_aux(struct skiplist *sl, void *val,
                                      sl_cmp_fn cmp, void *data, int c) {
    int c_next;
    struct skiplist *ret;

    assert(c > 0);

    if (SL_IS_HEAD(sl->next)) {
        c_next = -1;
    } else {
        c_next = cmp(val, sl->next->val_, data);
    }

    if (c_next <= 0) {
        if (sl->dup) {
            ret = sl_insert_aux(sl->dup, val, cmp, data, c);
            if (!ret) {
                return NULL;
            }
        } else {
            ret = sl_new_head();
            ret->val_ = val;
        }
        ret->next = sl->next;
        sl->next = ret;
        if (rand() > (RAND_MAX / 2)) {
            struct skiplist *new_ = sl_new_head();
            new_->val_ = val;
            new_->dup = ret;
            return new_;
        } else {
            return NULL;
        }
    } else {
        return sl_insert_aux(sl->next, val, cmp, data, c_next);
    }
}

SkipList sl_insert(SkipList sl, void *val) {
    struct skiplist *res;

    if (sl->bottom == NULL) {
        sl->top = sl->bottom = sl_new_head();
    }
    res = sl_insert_aux(sl->top, val, sl->cmp_, sl->data_, 1);
    if (res) {
        struct skiplist *tmp = sl->top;
        sl->top = sl_new_head();
        sl->top->next = res;
        res->next = sl->top;
        sl->top->dup = tmp;
    }

    return sl;
}

static void *sl_find_aux(struct skiplist *sl, void *val, sl_cmp_fn cmp,
                         void *data, int c) {
    int c_next;

    assert(c > 0);

    if (SL_IS_HEAD(sl->next)) {
        c_next = -1;
    } else {
        c_next = cmp(val, sl->next->val_, data);
    }

    if (c_next > 0) {
        return sl_find_aux(sl->next, val, cmp, data, c_next);
    } else if (c_next < 0) {
        if (sl->dup != NULL) {
            return sl_find_aux(sl->dup, val, cmp, data, c);
        } else {
            return NULL;
        }
    } else {
        return sl->next->val_;
    }
}

void *sl_find(SkipList sl, void *val) {
    if (sl->bottom == NULL) {
        return NULL;
    } else {
        return sl_find_aux(sl->top, val, sl->cmp_, sl->data_, 1);
    }
}

static void *sl_remove_aux(struct skiplist *sl, void *val, sl_cmp_fn cmp,
                           void *data, int c) {
    int c_next;
    struct skiplist *old;
    void *ret;

    assert(c > 0);

    if (SL_IS_HEAD(sl->next)) {
        c_next = -1;
    } else {
        c_next = cmp(val, sl->next->val_, data);
    }

    if (c_next > 0) {
        return sl_remove_aux(sl->next, val, cmp, data, c_next);
    } else if (c_next < 0) {
        if (sl->dup != NULL) {
            return sl_remove_aux(sl->dup, val, cmp, data, c);
        } else {
            return NULL;
        }
    } else {
        if (sl->dup != NULL) {
            old = sl->next;
            sl->next = sl->next->next;
            free(old);
            return sl_remove_aux(sl->dup, val, cmp, data, c);
        } else {
            old = sl->next;
            ret = old->val_;
            sl->next = sl->next->next;
            free(old);
            return ret;
        }
    }
}

void *sl_remove(SkipList sl, void *val) {
    void *ret;

    if (sl->bottom == NULL) {
        return NULL;
    } else {
        struct skiplist *old;
        ret = sl_remove_aux(sl->top, val, sl->cmp_, sl->data_, 1);
        while (sl->top->next == sl->top && sl->top->dup) {
            old = sl->top;
            sl->top = sl->top->dup;
            free(old);
        }
        return ret;
    }
}

static void sl_delete_aux(struct skiplist *sl, sl_free_fn free_fn) {
    if (free_fn && !SL_IS_HEAD(sl)) {
        free_fn(sl->val_);
    }

    if (sl->next && !SL_IS_HEAD(sl->next)) {
        sl_delete_aux(sl->next, free_fn);
    }

    free(sl);
}

void sl_delete(SkipList sl, sl_free_fn free_fn) {
    struct skiplist *dup, *cur = sl->top;

    while (cur != sl->bottom) {
        dup = cur->dup;
        sl_delete_aux(cur, NULL);
        cur = dup;
    }
    sl_delete_aux(cur, free_fn);

    free(sl);
}

static void sl_walk_aux(struct skiplist *sl, sl_visit_fn visit, void *arg) {
    if (!SL_IS_HEAD(sl)) {
        visit(sl->val_, arg);
        sl_walk_aux(sl->next, visit, arg);
    }
}

void sl_walk(SkipList sl, sl_visit_fn visit, void *arg) {
    sl_walk_aux(sl->bottom->next, visit, arg);
}
