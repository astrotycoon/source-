
#include <stdlib.h>

#include <sll.h>

typedef struct _set
    {
    BTREE_T* members;
    NODE_DATA_TYPE_E data_type;
    unsigned long num_mem;
    } SET_T;

typedef enum {
   SET_ERR_ERROR_HIGH = -512, /* fencepost */
   SET_ERR_MALLOC_FAIL,
   SET_ERR_DATA_TYPE_MISMATCH,
   SET_ERR_INCORRECT_DATA_TYPE,
   SET_ERR_ERR = -1, /* generic operational error */
   SET_ERR_OK = 0 /* routine returned without any errors */
} SET_ERR_E;


/*******************************************************************************
 * set_create - create a set
 *
 * RETURNS: SET_T* if ok; NULL if error
 */

int set_add_object (SET_T* s, void* mem)    
{
   sll_insert (s->sll, mem);   
}

/*******************************************************************************
 * set_create - create a set
 *
 * RETURNS: SET_T* if ok; NULL if error
 */

int set_add_string (SET_T* s, char* mem)    
{
   sll_insert (s->sll, mem);   
}

/*******************************************************************************
 * set_create - create a set
 *
 * RETURNS: SET_T* if ok; NULL if error
 */

int set_add_char (SET_T* s, char mem)    
{
   sll_insert (s->sll, mem);      
}

/*******************************************************************************
 * set_create - create a set
 *
 * RETURNS: SET_T* if ok; NULL if error
 */

SET_ERR_E set_add_int (SET_T* s, int mem)    
    {
    if (s->data_type != IDATA)
        return SET_ERR_INCORRECT_DATA_TYPE;
    btree_insert (s->members, 1, IDATA, mem);
    s->num_mem++;
    }

/*******************************************************************************
 * set_union_cb - create a set
 *
 * RETURNS: SET_T* if ok; NULL if error
 */

SET_T* set_union_cb
    (
    BTREE_NODE_T* node,
    const SET_T* set
    )    
    {   
    btree_find (
    }

/*******************************************************************************
 * set_union - create a set
 *
 * RETURNS: SET_T* if ok; NULL if error
 */

SET_T* set_union
    (
    const SET_T* fset, /* first set */
    const SET_T* sset /* second set */
    )    
    {   
    if (fset->data_type != IDATA)
        return SET_ERR_DATA_TYPE_MISMATCH;

    
    }


/*******************************************************************************
 * set_create - create a set
 *
 * RETURNS: SET_T* if ok; NULL if error
 */

SET_T* set_create (NODE_DATA_TYPE_E data_type)    
{  
   SET_T* set = malloc (sizeof (SET_T));
   if (NULL == set)
      return NULL;
   set->data_type = data_type;
   set->members = btree_new (0, 0);
   return set;
}
