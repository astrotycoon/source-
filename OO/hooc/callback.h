#include <stdarg.h>
#include <stdbool.h>
#include <sys/cdefs.h>

#ifndef __CALLBACK_H__
#define __CALLBACK_H__
__BEGIN_DECLS

/*
 * structure declaration
 */

typedef struct list_t list_t;
typedef bool (*func_t)(void*);
typedef bool (*cb_func_t)(func_t func, void* data, va_list ap);

/*
 * structure definition
 */

struct list_t {
    bool    valid;
    func_t  func;
    void*   data;
    list_t* prev;
    list_t* next;
};

/*
 * function declaration
 */

bool add_func(list_t** list, func_t func, void* data);
bool remove_func(list_t** list, func_t func);
bool remove_all_func(list_t** list);
bool invoke_all_func(list_t** list, cb_func_t func, ...);

__END_DECLS
#endif // __CALLBACK_H__
