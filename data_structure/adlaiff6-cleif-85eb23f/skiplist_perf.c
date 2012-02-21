#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <skiplist.h>

int int_cmp(const void *p1, const void *p2,
              const __attribute__((unused)) void *data) {
    return (*((int *) p1) > *((int *) p2)) - (*((int *) p1) < *((int *) p2));
}

static void shuffle(int a[], int n) {
    int k;
    while (--n > 0) {
        k = rand() % n;
        a[k] ^= a[n]; a[n] ^= a[k]; a[k] ^= a[n];
    }
}

int main(void) {
    SkipList sl;
    struct timespec tic, toc;
    double time_spent;
    int res, i, *v, *w, nelts, *found;
    const int lo = 8192, hi = 1048576/*8388608*/, factor = 2;

    printf("Inserting:\n");
    for (nelts = lo; nelts <= hi; nelts *= factor) {
        sl = sl_new(int_cmp, NULL);
        assert(sl);

        v = (int *) malloc(nelts * sizeof(*v));
        assert(v);

        for (i = 0; i < nelts; ++i) {
            v[i] = i;
        }

        shuffle(v, nelts);

        res = clock_gettime(CLOCK_REALTIME, &tic);
        assert(!res);
        for (i = 0; i < nelts; ++i) {
            sl = sl_insert(sl, &v[i]);
        }
        res = clock_gettime(CLOCK_REALTIME, &toc);
        assert(!res);

        time_spent = toc.tv_sec - tic.tv_sec +
            (toc.tv_nsec - tic.tv_nsec) / 1000000000.0;
        printf("%8d %12.08f\n", nelts, time_spent);

        free(v);
        sl_delete(sl, NULL);
    }

    printf("Finding:\n");
    for (nelts = lo; nelts <= hi; nelts *= factor) {
        sl = sl_new(int_cmp, NULL);
        assert(sl);

        v = (int *) malloc(nelts * sizeof(*v));
        assert(v);

        for (i = 0; i < nelts; ++i) {
            v[i] = i;
        }

        shuffle(v, nelts);

        for (i = 0; i < nelts; ++i) {
            sl = sl_insert(sl, &v[i]);
        }

        w = (int *) malloc(nelts * sizeof(*w));
        assert(w);

        for (i = 0; i < nelts; ++i) {
            w[i] = i;
        }

        shuffle(w, nelts);

        res = clock_gettime(CLOCK_REALTIME, &tic);
        assert(!res);
        for (i = 0; i < nelts; ++i) {
            found = (int *) sl_find(sl, &w[i]);
            assert(found);
            assert(*found == w[i]);
        }
        res = clock_gettime(CLOCK_REALTIME, &toc);
        assert(!res);

        time_spent = toc.tv_sec - tic.tv_sec +
            (toc.tv_nsec - tic.tv_nsec) / 1000000000.0;
        printf("%8d %12.08f\n", nelts, time_spent);

        free(w);
        free(v);
        sl_delete(sl, NULL);
    }

    printf("Removing:\n");
    for (nelts = lo; nelts <= hi; nelts *= factor) {
        sl = sl_new(int_cmp, NULL);
        assert(sl);

        v = (int *) malloc(nelts * sizeof(*v));
        assert(v);

        for (i = 0; i < nelts; ++i) {
            v[i] = i;
        }

        shuffle(v, nelts);

        for (i = 0; i < nelts; ++i) {
            sl = sl_insert(sl, &v[i]);
        }

        w = (int *) malloc(nelts * sizeof(*w));
        assert(w);

        for (i = 0; i < nelts; ++i) {
            w[i] = i;
        }

        shuffle(w, nelts);

        res = clock_gettime(CLOCK_REALTIME, &tic);
        assert(!res);
        for (i = 0; i < nelts; ++i) {
            found = (int *) sl_remove(sl, &w[i]);
            assert(found);
            assert(*found == w[i]);
        }
        res = clock_gettime(CLOCK_REALTIME, &toc);
        assert(!res);

        time_spent = toc.tv_sec - tic.tv_sec +
            (toc.tv_nsec - tic.tv_nsec) / 1000000000.0;
        printf("%8d %12.08f\n", nelts, time_spent);

        free(w);
        free(v);
        sl_delete(sl, NULL);
    }

    printf("Deleting:\n");
    for (nelts = lo; nelts <= hi; nelts *= factor) {
        sl = sl_new(int_cmp, NULL);
        assert(sl);

        v = (int *) malloc(nelts * sizeof(*v));
        assert(v);

        for (i = 0; i < nelts; ++i) {
            v[i] = i;
        }

        shuffle(v, nelts);

        for (i = 0; i < nelts; ++i) {
            sl = sl_insert(sl, &v[i]);
        }

        free(v);

        res = clock_gettime(CLOCK_REALTIME, &tic);
        assert(!res);
        sl_delete(sl, NULL);
        res = clock_gettime(CLOCK_REALTIME, &toc);
        assert(!res);

        time_spent = toc.tv_sec - tic.tv_sec +
            (toc.tv_nsec - tic.tv_nsec) / 1000000000.0;
        printf("%8d %12.08f\n", nelts, time_spent);
    }

    return 0;
}
