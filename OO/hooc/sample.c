#include <stdio.h>

#include "callback.h"

/*
 * user defined callback function
 */

typedef bool (*my_func_t)(const char*, const char*);

static bool func(const char*, const char*);

static bool my_cb_func(func_t func, void* data, va_list ap);

int main(int argc, char* argv[])
{
    list_t* list = NULL;

    add_func(&list, (func_t) func, (void*) "Alice");
    add_func(&list, (func_t) func, (void*) "Mary");
    add_func(&list, (func_t) func, (void*) "Lisa");

    invoke_all_func(&list, my_cb_func, "Hello");
    remove_all_func(&list);

    return 0;
}

static bool my_cb_func(func_t func, void* data, va_list ap)
{
    my_func_t my_func = (my_func_t) func;
    const char* msg = va_arg(ap, const char*);
    return my_func((const char*) data, msg);
}

static bool func(const char* name, const char* msg)
{
    fprintf(stderr, "%s says '%s'.\n", name, msg);
    return true;
}
