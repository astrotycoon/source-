#include <assert.h>

#include <unittest.h>
#include <withstmt.h>

struct null_lock {
    int id, num_locks, num_unlocks;
};

static inline void nl_init(struct null_lock *lock, int id) {
    lock->num_locks = lock->num_unlocks = 0;
    lock->id = id;
}

static void nl_lock(struct null_lock *lock) {
    lock->num_locks++;
}

static void nl_unlock(struct null_lock *lock) {
    lock->num_unlocks++;
}

static void test_single_lock(void) {
    struct null_lock lock;

    nl_init(&lock, 1);

    expect(lock.num_locks == 0);
    expect(lock.num_unlocks == 0);

    with(lock, nl_lock, nl_unlock, {
            assert(lock.num_locks == 1);
            assert(lock.num_unlocks == 0);
        });

    assert(lock.num_locks == 1);
    assert(lock.num_unlocks == 1);
}

static void test_double_lock(void) {
    struct null_lock lock1, lock2;

    nl_init(&lock1, 1);
    nl_init(&lock2, 2);

    expect(lock1.num_locks == 0);
    expect(lock1.num_unlocks == 0);
    expect(lock2.num_locks == 0);
    expect(lock2.num_unlocks == 0);

    with(lock1, nl_lock, nl_unlock, {
            assert(lock1.num_locks == 1);
            assert(lock1.num_unlocks == 0);
        });

    assert(lock1.num_locks == 1);
    assert(lock1.num_unlocks == 1);

    with(lock2, nl_lock, nl_unlock, {
            assert(lock2.num_locks == 1);
            assert(lock2.num_unlocks == 0);
            expect(lock1.num_locks == 1);
            expect(lock1.num_unlocks == 1);

            with(lock1, nl_lock, nl_unlock, {
                    expect(lock2.num_locks == 1);
                    expect(lock2.num_unlocks == 0);
                    assert(lock1.num_locks == 2);
                    assert(lock1.num_unlocks == 1);
                });

            expect(lock2.num_locks == 1);
            expect(lock2.num_unlocks == 0);
            assert(lock1.num_locks == 2);
            assert(lock1.num_unlocks == 2);
        });

    assert(lock2.num_locks == 1);
    assert(lock2.num_unlocks == 1);
    assert(lock1.num_locks == 2);
    assert(lock1.num_unlocks == 2);
}

int main(void) {
    TEST(single_lock);
    TEST(double_lock);

    return 0;
}
