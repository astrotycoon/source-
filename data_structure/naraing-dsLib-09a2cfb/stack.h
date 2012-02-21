/* stack.c - stack implementation */

/*
modification history
--------------------
05jul08,gnm written
*/

/*
DESCRIPTION

*/

/* Stack data */
typedef int STACK_ELE_T;

/* Stack */
typedef struct _stack
    {
    SLL_T* sll;
    unsigned long no_ele;    
    }STACK_T;

/* Stack iterator function that stack data operates on */
typedef void (*STACK_ITER_FN)(NODE_DATA_T);

/* Prototypes/forward declarations */
STACK_T* stack_new (void);
int      stack_delete (STACK_T*);
int      stack_push (STACK_T* S, STACK_ELE_T);
int      stack_pop (STACK_T*);
BOOL     stack_empty (STACK_T*);
void     stack_iter (STACK_T*, STACK_ITER_FN);
void     iter_fn_print (STACK_ELE_T);
