#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Object.h"

/*
 * Declaration
 */
typedef struct ObjectData ObjectData;
typedef struct ObjectList ObjectList;

struct ObjectData {
    ObjectList* list;
    int counter;
};

struct ObjectList {
    ObjectList* next;
    ObjectList* prev;
    Object* instance;
};

static inline ObjectData* getObjectData(Object* self)
{
    return (ObjectData*) self->_priv;
}

static inline void addToList(Object* self)
{
    ObjectData* data = getObjectData(self);
    ObjectList* list = malloc(sizeof(ObjectList));
    memset(list, 0, sizeof(ObjectList));
    list->instance = self;

    if (data->list == NULL) {
        data->list = list;
    } else {
        list->next = data->list;
        data->list->prev = list;
        data->list = list;
    }
}

static inline void removeFromList(Object* self)
{
    ObjectData* data = getObjectData(self);
    ObjectList* list = data->list;

    while (list != NULL) {
        if (list->instance == self) {
            if (list->prev == NULL) {
                data->list = list->next;
            } else {
                list->prev->next = list->next;
                if (list->next != NULL) list->next->prev = list->prev;
            }
            free(list);
            break;
        }
        list = list->next;
    }
}

/**
 * @sa Object::ref
 */
static Object* Object_ref(Object* self);

/**
 * @sa Object::unref
 */
static void Object_unref(Object* self);

/**
 * @sa Object::destroy
 */
static void Object_destroy(Object* self);

/**
 * @sa Object::funcWithoutArgumentNoReturn
 */
static void Object_funcWithoutArgumentNoReturn(Object* self);

/**
 * @sa Object::funcWithArgumentReturnBool
 */
static bool Object_funcWithArgumentReturnBool(Object* self, int argA, char* argB);

/**
 * @brief A private function of Object
 * @private @memberof Object
 * @fn void privateFunction(Object* self);
 * @param self An instance of Object.
 */
static void Object_privateFunction(Object* self);

static const Object object = {
    ._priv   = NULL,
    .ref     = Object_ref,
    .unref   = Object_unref,
    .destroy = Object_destroy,
    .funcWithoutArgumentNoReturn = Object_funcWithoutArgumentNoReturn,
    .funcWithArgumentReturnBool  = Object_funcWithArgumentReturnBool,
};

/*
 * Function Implementation
 */

Object* Object_create(void)
{
    ObjectData* data = malloc(sizeof(ObjectData)); printf("create object data %p\n", data);
    Object* self = malloc(sizeof(Object)); printf("create object %p\n", self);

    memset(data, 0, sizeof(ObjectData));
    memcpy(self, &object, sizeof(Object));
    self->_priv = (void*) data;
    data->counter++;
    addToList(self);

    return self;
}

static Object* Object_ref(Object* self)
{
    Object* clone = malloc(sizeof(Object));
    ObjectData* data = getObjectData(self);
    memcpy(clone, self, sizeof(Object)); printf("clone object %p\n", clone);
    data->counter++;
    addToList(clone);
    return clone;
}

static void Object_unref(Object* self)
{
    ObjectData* data = getObjectData(self);
    removeFromList(self);
    free(self); printf("free object %p\n", self);
    data->counter--;
    if (data->counter == 0) {
        free(data); printf("free object data %p\n", data);
    }
}

static void Object_destroy(Object* self)
{
    ObjectData* data = getObjectData(self);
    while (data->list != NULL) {
        ObjectList* swap = data->list->next;
        free(data->list->instance); printf("free object %p\n", data->list->instance);
        data->list = swap;
    }
    free(data); printf("free object data %p\n", data);
}

static void Object_funcWithoutArgumentNoReturn(Object* self)
{
    Object_privateFunction(self);
    return;
}

static bool Object_funcWithArgumentReturnBool(Object* self, int argA, char* argB)
{
    return false;
}

static void Object_privateFunction(Object* self)
{
    return;
}
