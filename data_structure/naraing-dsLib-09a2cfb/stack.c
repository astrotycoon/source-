/* stack.c - stack implementation */

/*
modification history
--------------------
07jul08,gnm written
*/

/*
DESCRIPTION

*/

#include <stdio.h>
#include <stdlib.h>

#include <common.h>
#include <sll.h>
#include <stack.h>

#undef STACK_TEST

int stack_delete
    (
    STACK_T* S
    )
    {
    if (NULL == S)
        return 0;
    
    while (S->sll->lhead)
        sll_delete (S->sll, S->sll->lhead->item);
    
    free (S->sll);
    free (S);
    return 0;
    }

STACK_T* stack_new (void)
    {
    STACK_T* S;

    S = (STACK_T*)malloc (sizeof (STACK_T));
    if (NULL == S)
        return NULL;
    S->sll = sll_new(IDATA, NULL, NULL, NULL);
    if (NULL == S->sll)
        {
        free (S);
        return NULL;
        }
    S->no_ele = 0;
    return S;       
    }

BOOL stack_empty
    (
    STACK_T* S
    )
    {
    return ((S->sll->lhead == NULL)? TRUE: FALSE);
    }

int stack_push
    (
    STACK_T* S,
    STACK_ELE_T ele
    )
    {
    if (NULL == S)
        return -1;
    
    sll_insert (S->sll, (int)ele);    
    return 0;
    }

void stack_iter
    (
    STACK_T* S,
    STACK_ITER_FN fn
    )
    {
    SLL_NODE* node;

    node = S->sll->lhead;
    while (node)        
        {
        fn (node->item);
        node = node->next;
        }    
    }

/*
 * Returns: 0 if operation successful
 *          -1 if error
 *          -2 if stack empty
 */

int stack_pop
    (
    STACK_T* S
    )
    {
    SLL_NODE* node;
    
    if (NULL == S || NULL == S->sll->lhead)
        return -1;

    node = sll_delete (S->sll, (S->sll->ltail)->item);
    return (node->item);
    }

void iter_fn_print (STACK_ELE_T ele)
    {
    fprintf (stdout, "0x%x\n", ele);
    }

#ifdef STACK_TEST
int main (int argc, char** argv)
    {
    STACK_T* S;
    char* a;
    int i;
    
    S = stack_new ();

    for (i = 0; i < 10; i++)
        {
        a = malloc (10);
        fprintf (stdout, "pushing %p\n", a);
        stack_push (S, (int)a);
        }

    while (FALSE == stack_empty(S))
        {
        a = (char*)stack_pop (S);
        fprintf (stdout, "pop'd %p\n", a);
        }
    return 0;
    }
    
#endif /* defined STACK_TEST */
