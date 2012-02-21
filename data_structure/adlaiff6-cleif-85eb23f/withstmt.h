#ifndef __WITHSTMT_H__
#define __WITHSTMT_H__

#define with(lock, lock_fn, unlock_fn, block)   \
    do {                                        \
        lock_fn(&lock);                         \
        (block);                                \
        unlock_fn(&lock);                       \
    } while(0)

#endif  /* __WITHSTMT_H__ */
