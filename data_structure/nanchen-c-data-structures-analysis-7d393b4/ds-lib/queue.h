typedef void* ElementType;

#ifndef _Queue_h
#define _Queue_h

struct QueueRecord;
typedef struct QueueRecord *Queue;

int Queue_isEmpty(Queue Q);
int Queue_isFull(Queue Q);
Queue Queue_createQueue(int MaxElements);
void Queue_disposeQueue(Queue Q);
void Queue_makeEmpty(Queue Q);
void Queue_enqueue(ElementType X, Queue Q);
ElementType Queue_front(Queue Q);
void Queue_dequeue(Queue Q);
ElementType Queue_frontAndDequeue(Queue Q);

#endif  /* _Queue_h */
/* END */
