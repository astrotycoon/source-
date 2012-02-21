#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <klist.h>
#include <unittest.h>

struct item {
    int val;
    struct klist head;
};

static void test_init(void) {
    LIST_HEAD(head);

    assert(lempty(&head));
}

static void test_add(void) {
    int i;
    LIST_HEAD(head);
    struct klist *p;
    struct item *e;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd(&elt->head, &head);
    }

    e = lentry_first(&head, struct item, head);
    for (i = 99; i >= 0; --i) {
        assert(e->val == i);
        e = lentry_first(&e->head, struct item, head);
    }

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        free(lentry(p, struct item, head));
    }
}

static void test_add_tail(void) {
    int i;
    LIST_HEAD(head);
    struct klist *p;
    struct item *e;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd_tail(&elt->head, &head);
    }

    e = lentry_first(&head, struct item, head);
    for (i = 0; i < 100; ++i) {
        assert(e->val == i);
        e = lentry_first(&e->head, struct item, head);
    }

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        free(lentry(p, struct item, head));
    }
}

static void test_head(void) {
    int i;
    LIST_HEAD(head);
    struct klist *p;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd_tail(&elt->head, &head);
    }

    assert(lentry(lhead(&head), struct item, head)->val == 0);

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        free(lentry(p, struct item, head));
    }
}

static void test_tail(void) {
    int i;
    LIST_HEAD(head);
    struct klist *p;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd_tail(&elt->head, &head);
    }

    assert(lentry(ltail(&head), struct item, head)->val == 99);

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        free(lentry(p, struct item, head));
    }
}

static void test_push(void) {
    int i;
    LIST_HEAD(head);
    struct klist *p;
    struct item *e;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	lpush(&elt->head, &head);
    }

    e = lentry_first(&head, struct item, head);
    for (i = 99; i >= 0; --i) {
        assert(e->val == i);
        e = lentry_first(&e->head, struct item, head);
    }

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        free(lentry(p, struct item, head));
    }
}

static void test_enqueue(void) {
    int i;
    LIST_HEAD(head);
    struct klist *p;
    struct item *e;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	lenqueue(&elt->head, &head);
    }

    e = lentry_first(&head, struct item, head);
    for (i = 99; i >= 0; --i) {
        assert(e->val == i);
        e = lentry_first(&e->head, struct item, head);
    }

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        free(lentry(p, struct item, head));
    }
}

static void test_pop(void) {
    int i;
    LIST_HEAD(head);
    struct klist *p;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd_tail(&elt->head, &head);
    }

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        assert(p);
        assert(lentry(p, struct item, head));
        assert(lentry(p, struct item, head)->val == i);
        free(lentry(p, struct item, head));
    }
}

static void test_dequeue(void) {
    int i;
    LIST_HEAD(head);
    struct klist *p;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd(&elt->head, &head);
    }

    for (i = 0; i < 100; ++i) {
        p = dequeue(&head);
        assert(p);
        assert(lentry(p, struct item, head));
        assert(lentry(p, struct item, head)->val == i);
        free(lentry(p, struct item, head));
    }
}

static void test_splice(void) {
    int i;
    LIST_HEAD(head1);
    LIST_HEAD(head2);
    struct klist *p, *splice;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	lenqueue(&elt->head, &head1);
    }

    for (; i < 200; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
        lenqueue(&elt->head, &head2);
    }

    splice = lsplice(&head2, &head1);
    assert(splice);

    for (i = 0; i < 200; ++i) {
        p = dequeue(splice);
        assert(p);
        assert(lentry(p, struct item, head));
        assert(lentry(p, struct item, head)->val == i);
        free(lentry(p, struct item, head));
    }
}

static void test_foreach(void) {
    int i, last;
    LIST_HEAD(head);
    struct klist *p;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd_tail(&elt->head, &head);
    }

    last = -1;
    lforeach(p, &head) {
        assert(lentry(p, struct item, head)->val - 1 == last);
        last = lentry(p, struct item, head)->val;
    }
    assert(last == 99);

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        expect(lentry(p, struct item, head)->val == i);
        free(lentry(p, struct item, head));
    }
}

static void test_foreach_rev(void) {
    int i, last;
    LIST_HEAD(head);
    struct klist *p;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd_tail(&elt->head, &head);
    }

    last = 100;
    lforeach_rev(p, &head) {
        assert(lentry(p, struct item, head)->val + 1 == last);
        last = lentry(p, struct item, head)->val;
    }
    assert(last == 0);

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        expect(lentry(p, struct item, head)->val == i);
        free(lentry(p, struct item, head));
    }
}

static void test_foreach_entry(void) {
    int i, last;
    LIST_HEAD(head);
    struct klist *p;
    struct item *e;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd_tail(&elt->head, &head);
    }

    last = -1;
    lforeach_entry(e, &head, struct item, head) {
        assert(e->val - 1 == last);
        last = e->val;
    }
    assert(last == 99);

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        expect(lentry(p, struct item, head)->val == i);
        free(lentry(p, struct item, head));
    }
}

static void test_foreach_entry_rev(void) {
    int i, last;
    LIST_HEAD(head);
    struct klist *p;
    struct item *e;

    for (i = 0; i < 100; ++i) {
	struct item *elt = (struct item *) malloc(sizeof(*elt));
	expect(elt);
	elt->val = i;
	ladd_tail(&elt->head, &head);
    }

    last = 100;
    lforeach_entry_rev(e, &head, struct item, head) {
        assert(e->val + 1 == last);
        last = e->val;
    }
    assert(last == 0);

    for (i = 0; i < 100; ++i) {
        p = pop(&head);
        expect(p);
        expect(lentry(p, struct item, head));
        expect(lentry(p, struct item, head)->val == i);
        free(lentry(p, struct item, head));
    }
}

int main(void)
{
    TEST(init);
    TEST(add);
    TEST(add_tail);
    TEST(head);
    TEST(tail);
    TEST(push);
    TEST(enqueue);
    TEST(pop);
    TEST(dequeue);
    TEST(splice);
    TEST(foreach);
    TEST(foreach_rev);
    TEST(foreach_entry);
    TEST(foreach_entry_rev);

    return 0;
}
