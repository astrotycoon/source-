#include <sys/cdefs.h> /* __BEGIN_DECLS __END_DECLS */
#include <linux/stddef.h> /* NULL*/
#include <stdbool.h> /* bool true false */
#ifndef __OBJECT_H__
#define __OBJECT_H__
__BEGIN_DECLS

/**
 * @class Object
 * @brief The brief description of Object.
 *
 * The detailed description of Object.<BR/>
 * Use Object_create() to create the instance of Object.
 */
typedef struct Object Object;

/**
 * @brief The necessary constructor of Object.
 *
 * @return An instance of Object.
 * @retval NULL If it fails.
 */
Object* Object_create(void);

struct Object {
    /** @cond */
    void* _priv;
    /** @endcond */

    /**
     * @brief Increases the reference count of Object.
     * @public @memberof Object
     * @fn Object* ref(Object* self);
     * @param self An instance of Object.
     * @return Object*
     */
    /** @cond */
    Object* (*ref)(Object* self);
    /** @endcond */

    /**
     * @brief Decreases the reference count of Object.
     * @public @memberof Object
     * @fn void unref(Object* self);
     * @param self An instance of Object.
     */
    /** @cond */
    void (*unref)(Object* self);
    /** @endcond */

    /**
     * @brief The necessary destructor of Object
     * @public @memberof Object
     * @fn void destroy(Object* self);
     * @param self An instance of Object.
     */
    /** @cond */
    void (*destroy)(Object* self);
    /** @endcond */

    /**
     * @brief A public function without argument and no return.
     * @public @memberof Object
     * @fn void funcWithoutArgumentNoReturn(Object* self);
     * @param self An instance of Object.
     */
    /** @cond */
    void (*funcWithoutArgumentNoReturn)(Object* self);
    /** @endcond */

    /**
     * @brief A public function with argument and return bool.
     * @public @memberof Object
     * @fn bool funcWithArgumentReturnBool(Object* self, int argA, char* argB);
     * @param self An instance of Object.
     * @param argA An integer argument.
     * @param argB A character string argument.
     * @return bool
     * @retval true If it works.
     * @retval false If it fails.
     */
    /** @cond */
    bool (*funcWithArgumentReturnBool)(Object* self, int argA, char* argB);
    /** @endcond */
};

__END_DECLS
#endif //__OBJECT_H__
