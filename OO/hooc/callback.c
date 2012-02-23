#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "callback.h"

/*
 * static variable
 */

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * function definition
 */

bool add_func(list_t** list, func_t func, void* data)
{
    list_t* item = NULL;
    list_t* ptr = NULL;

    if (list == NULL || func == NULL) return false;

    pthread_mutex_lock(&mutex);

    item = *list;

    ptr = (list_t*) malloc(sizeof(list_t));
    memset(ptr, 0, sizeof(list_t));
    ptr->func = func;
    ptr->data = data;
    ptr->valid = true;

    if (item == NULL) {
        ptr->next = ptr;
        ptr->prev = ptr;
        *list = ptr;
        pthread_mutex_unlock(&mutex);
        return true;
    }

    item->prev->next = ptr;
    ptr->prev = item->prev;
    item->prev = ptr;
    ptr->next = item;

    pthread_mutex_unlock(&mutex);
    return true;
}

bool remove_func(list_t** list, func_t func)
{
    list_t* item = NULL;

    if (list == NULL || func == NULL || *list == NULL) return false;

    pthread_mutex_lock(&mutex);

    item = *list;

    do {
        if (item->func == func && item->valid == true) {
            item->valid = false;
            pthread_mutex_unlock(&mutex);
            return true;
        }
        item = item->next;
    } while (item != *list && item->func != func);

    pthread_mutex_unlock(&mutex);
    return false;
}

bool remove_all_func(list_t** list)
{
    list_t* item = NULL;
    pthread_mutex_lock(&mutex);

    if (*list == NULL) {
        pthread_mutex_unlock(&mutex);
        return true;
    }

    item = *list;

    do {
        list_t* ptr = item->next;
        free(item);
        item = ptr;
    } while (item != *list);

    *list = NULL;

    pthread_mutex_unlock(&mutex);
    return true;
}

bool invoke_all_func(list_t** list, cb_func_t func, ...)
{
    list_t* ptr = NULL;
    va_list ap;

    if (list == NULL) return false;

    pthread_mutex_lock(&mutex);

    if (*list == NULL) {
        pthread_mutex_unlock(&mutex);
        return true;
    }

    va_start(ap, func);

    ptr = (*list)->prev;

    while (ptr != *list) {
        if (ptr->valid == true) {
            func_t tmp = ptr->func;
            void* data = ptr->data;
            pthread_mutex_unlock(&mutex);
            if (func(tmp, data, ap) == false) {
                va_end(ap);
                return false;
            }
            pthread_mutex_lock(&mutex);
        }
        if (*list != NULL) {
            ptr = ptr->prev;
        } else {
            pthread_mutex_unlock(&mutex);
            va_end(ap);
            return true;
        }
    }

    if (ptr->valid == true) {
        func_t tmp = ptr->func;
        void* data = ptr->data;
        pthread_mutex_unlock(&mutex);
        if (func(tmp, data, ap) == false) {
            va_end(ap);
            return false;
        }
        pthread_mutex_lock(&mutex);
    }

    va_end(ap);
    pthread_mutex_unlock(&mutex);

    return true;
}
