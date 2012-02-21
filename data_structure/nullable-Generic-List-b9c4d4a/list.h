#ifndef __STACK_H__
#define __STACK_H__

#include <stdint.h>

struct list_bucket
{
    int16_t type;
    void* element;
};

typedef struct generic_list_s{
  struct list_bucket** array;
  int32_t start;
  int32_t count;
  int32_t capacity;
} list;

typedef struct generic_list_s stack;
typedef struct generic_list_s queue;

extern struct generic_list_s *new_generic_list(int32_t initial);
extern list* new_generic_list(int32_t initial);
extern stack* new_stack(int32_t initial);
extern queue* new_queue(int32_t initial);

extern void* set_element_with_type_at(list *l, void* obj, int16_t type, int32_t pos);
extern void* set_element_at(list *l, void* obj, int32_t pos);

extern void insert_element_with_type_at(list* l, void* obj, int16_t type, int32_t pos);
extern void insert_element_at(list* l, void* obj, int32_t pos);

extern void append_element(list* l, void* obj, int16_t type);
extern void prepend_element(list* l, void* obj, int16_t type);

extern void add_element_with_type(list* l, void* obj, int16_t type);
extern void add_element(list* l, void* obj);

extern void* get_element_at(list* l, int32_t pos);
extern void* get_element_and_type_at(list* l, int32_t pos, int16_t* type);

extern int32_t remove_element(list *l, void* obj);
extern int32_t remove_all(list *l, void* obj);
extern void* remove_element_at(list* l, int32_t pos);

extern void enqueue_with_type(queue* q, void* obj, int16_t type);
extern void enqueue(queue* q, void* obj);
extern void* dequeue(queue* q);

extern void push_stack_type(stack* s, void* obj, int16_t type);
extern void push_stack(stack* s, void* obj);
extern void* pop_stack(stack* s);

extern void reverse(struct generic_list_s *s);

extern int16_t peek_element_type_at(list* l, int32_t pos);
extern int16_t peek_stack_type(stack *s);
extern int16_t peek_queue_type(queue *s);

extern struct generic_list_s *merge_lists(struct generic_list_s *l1, struct generic_list_s *l2);

extern void destroy_generic_list(struct generic_list_s *s);
#endif

