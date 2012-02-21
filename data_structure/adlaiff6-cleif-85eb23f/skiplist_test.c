#include <assert.h>
#include <stdlib.h>

#include <skiplist.h>
#include <unittest.h>

static void shuffle(int a[], int n) {
    int k;
    while (--n > 0) {
        k = rand() % n;
        a[k] ^= a[n]; a[n] ^= a[k]; a[k] ^= a[n];
    }
}

int int_cmp(const void *p1, const void *p2,
              const __attribute__((unused)) void *data) {
    return (*((int *) p1) > *((int *) p2)) - (*((int *) p1) < *((int *) p2));
}

static void test_create(void) {
    SkipList sl;

    sl = NULL;
    sl = sl_new(int_cmp, NULL);
    assert(sl);

    sl_delete(sl, NULL);
}

static void test_insert(void) {
    SkipList sl, tmp_sl;
    int i, *v;
    const int nelts = 1000;

    sl = sl_new(int_cmp, NULL);
    expect(sl);

    v = (int *) malloc(nelts * sizeof(*v));
    expect(v);

    shuffle(v, nelts);

    for (i = 0; i < nelts; ++i) {
        v[i] = i;
        tmp_sl = NULL;
        tmp_sl = sl_insert(sl, &v[i]);
        assert(tmp_sl);
    }

    sl_delete(sl, NULL);
    free(v);
}

static void test_find(void) {
    SkipList sl, tmp_sl;
    int i, *v, *w, *found;
    const int nelts = 1000;

    sl = sl_new(int_cmp, NULL);
    expect(sl);

    v = (int *) malloc(nelts * sizeof(*v));
    expect(v);

    for (i = 0; i < nelts; ++i) {
        v[i] = i;
    }

    shuffle(v, nelts);

    for (i = 0; i < nelts; ++i) {
        tmp_sl = NULL;
        tmp_sl = sl_insert(sl, &v[i]);
        expect(tmp_sl);
    }

    w = (int *) malloc(nelts * sizeof(*w));
    expect(w);

    for (i = 0; i < nelts; ++i) {
        w[i] = i;
    }

    shuffle(w, nelts);

    for (i = 0; i < nelts; ++i) {
        found = (int *) sl_find(sl, &w[i]);
        assert(found);
        assert(*found == w[i]);
    }

    sl_delete(sl, NULL);
    free(v);
}

static void test_remove(void) {
    SkipList sl, tmp_sl;
    int i, j, *v, *w, *found;
    const int nelts = 100;

    sl = sl_new(int_cmp, NULL);
    expect(sl);

    v = (int *) malloc(nelts * sizeof(*v));
    expect(v);

    for (i = 0; i < nelts; ++i) {
        v[i] = i;
    }

    shuffle(v, nelts);

    for (i = 0; i < nelts; ++i) {
        tmp_sl = NULL;
        tmp_sl = sl_insert(sl, &v[i]);
        expect(tmp_sl);
    }

    w = (int *) malloc(nelts * sizeof(*w));
    expect(w);

    for (i = 0; i < nelts; ++i) {
        w[i] = i;
    }

    shuffle(w, nelts);

    for (i = 0; i < nelts; ++i) {
        found = (int *) sl_remove(sl, &w[i]);
        assert(found);
        assert(*found == w[i]);
        for (j = 0; j < nelts; ++j) {
            found = (int *) sl_find(sl, &w[j]);
            if (i == j) {
                assert(!found);
            } else {
                assert(found);
                assert(*found == w[j]);
            }
        }
        sl_insert(sl, &w[i]);
        found = (int *) sl_find(sl, &w[i]);
        expect(found);
        expect(*found == w[i]);
    }

    sl_delete(sl, NULL);
    free(v);
}

struct visitor_arg {
    int nelts;
    int *array;
};

void visitor(const void *val, void *arg) {
    int nelts = ((struct visitor_arg *) arg)->nelts;
    int i = *((int *) val);
    int *a = ((struct visitor_arg *) arg)->array;
    assert(i == 0 || a[i - 1]);
    assert(i == nelts - 1 || !a[i + 1]);
    a[i] = 1;
}

static void test_visit(void) {
    SkipList sl, tmp_sl;
    int i, *v;
    struct visitor_arg v_arg;
    const int nelts = 1000;

    sl = sl_new(int_cmp, NULL);
    expect(sl);

    v = (int *) malloc(nelts * sizeof(*v));
    expect(v);
    for (i = 0; i < nelts; ++i) {
        v[i] = i;
        tmp_sl = NULL;
        tmp_sl = sl_insert(sl, &v[i]);
        expect(tmp_sl);
    }

    v_arg.array = (int *) malloc(nelts * sizeof(*v_arg.array));
    expect(v_arg.array);

    for (i = 0; i < nelts; ++i) {
        v_arg.array[i] = 0;
    }

    v_arg.nelts = nelts;

    sl_walk(sl, (sl_visit_fn) visitor, &v_arg);

    for (i = 0; i < nelts; ++i) {
        assert(v_arg.array[i]);
    }

    sl_delete(sl, NULL);
    free(v);
}

int main(void) {
    TEST(create);
    TEST(insert);
    TEST(find);
    TEST(remove);
    TEST(visit);

    return 0;
}
