#ifndef __SKIPLIST_H__
#define __SKIPLIST_H__

struct skiplist {
    void *val_;
    struct skiplist *next, *dup;
};

typedef int (*sl_cmp_fn)(const void *, const void *, const void *);
typedef void (*sl_free_fn)(void *);
typedef int (*sl_visit_fn)(const void *, void *);

struct sl_meta {
    struct skiplist *top, *bottom;
    sl_cmp_fn cmp_;
    void *data_;
};
typedef struct sl_meta * SkipList;

void sl_init(SkipList, sl_cmp_fn, void *);
SkipList sl_new(sl_cmp_fn, void *);
SkipList sl_insert(SkipList, void *);
void *sl_find(SkipList, void *);
void *sl_remove(SkipList, void *);
void sl_delete(SkipList, sl_free_fn);
void sl_walk(SkipList, sl_visit_fn, void *);

#endif  /* __SKIPLIST_H__ */
