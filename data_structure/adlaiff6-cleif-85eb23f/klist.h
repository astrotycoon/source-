#ifndef _LIST_H_
#define _LIST_H_

#include <util.h>

struct klist {
    struct klist *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) struct klist name = LIST_HEAD_INIT(name)

#define lhead(head) (head)->next
#define ltail(head) (head)->prev

#define lentry(ptr, type, member) container_of(ptr, type, member)
#define lentry_first(l, type, member) lentry(lhead(l), type, member)

static inline void INIT_LIST_HEAD(struct klist *l)
{
    lhead(l) = ltail(l) = l;
}

static inline void __ladd(struct klist *elt,
	       	struct klist *prev, struct klist *next)
{
    next->prev = elt;
    elt->next = next;
    elt->prev = prev;
    prev->next = elt;
}

static inline void ladd(struct klist *elt, struct klist *l)
{
    __ladd(elt, l, lhead(l));
}

static inline void ladd_tail(struct klist *elt, struct klist *l)
{
    __ladd(elt, ltail(l), l);
}

#define lpush(elt, l) ladd((elt), (l))
#define lenqueue(elt, l) ladd((elt), (l))

static inline void __ldel(struct klist *prev, struct klist *next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void ldel(struct klist *entry)
{
    __ldel(entry->prev, entry->next);
    entry->next = entry->prev = NULL;
}

static inline struct klist *pop(struct klist *l)
{
    struct klist *elt = lhead(l);
    ldel(elt);
    return elt;
}

static inline struct klist *dequeue(struct klist *l)
{
    struct klist *elt = ltail(l);
    ldel(elt);
    return elt;
}

static inline int lempty(struct klist *l)
{
    return lhead(l) == l;
}

static inline struct klist *lsplice(struct klist *l1, struct klist *l2)
{
    ltail(l1)->next = lhead(l2);
    lhead(l2)->prev = ltail(l1);
    ltail(l2)->next = l1;
    l1->prev = ltail(l2);
    l2->next = l2->prev = NULL;
    return l1;
}

#define lforeach(pos, head) \
    for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

#define lforeach_rev(pos, head) \
    for ((pos) = (head)->prev; (pos) != (head); (pos) = (pos)->prev)

#define lforeach_entry(pos, head, type, member) \
    for ((pos) = lentry((head)->next, type, member); \
	    &(pos)->member != (head); \
	    (pos) = lentry((pos)->member.next, type, member))

#define lforeach_entry_rev(pos, head, type, member) \
    for ((pos) = lentry((head)->prev, type, member); \
	    &(pos)->member != (head); \
	    (pos) = lentry((pos)->member.prev, type, member))

#endif  /* _LIST_H_ */
