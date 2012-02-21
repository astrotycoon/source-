/* queue.h - header information for queue operations */

/*
Modification History
--------------------
08aug10,gnm written 
 
*/

#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include <ll.h>

#define QUEUE_DEPTH(Q)          ((Q)->cur_size)
#define QUEUE_SIZE(Q)           ((Q)->max_size)
#define QUEUE_IS_EMPTY(Q)       ((Q)->cur_size)?DS_FALSE:DS_TRUE
#define QUEUE_IS_FULL(Q)        ((Q)->cur_size==(Q)->max_size)?DS_TRUE:DS_FALSE


typedef enum {
   Q_ERR_ERROR_HIGH = -512, /* fencepost */
   Q_ERR_MALLOC_FAIL,
   Q_ERR_ERR = -1, /* generic operational error */
   Q_ERR_OK = 0, /* routine returned without any errors */
   Q_ERR_OVERFLOW,
   Q_ERR_UNDERFLOW
} QUEUE_ERR_E;

typedef struct {
   SLL_T* q;
   unsigned long max_size;
   unsigned long cur_size;
} QUEUE_T;

TRUTH_E queue_is_full (QUEUE_T* q);
TRUTH_E queue_is_empty (QUEUE_T* q);
QUEUE_T* queue_create (NODE_DATA_TYPE_E data_type, unsigned long size);
QUEUE_ERR_E queue_enqueue_vp (QUEUE_T* q, void* data);
void* queue_dequeue_vp (QUEUE_T* q, QUEUE_ERR_E* err);
#endif
