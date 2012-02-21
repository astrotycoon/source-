#include    <stdio.h>
#include    <stdlib.h>

#include    "clist.h"

list *
list_new (void)
{
    list * new = (list *) calloc (1, sizeof (list));
    new->values = (void **) calloc (1, _LIST_BLOCK * sizeof (void *));
    return new;
}

void
_list_append (list ** lst,
        void * value)
{
    if (!((*lst)->length % _LIST_BLOCK))
        (*lst)->values = (void **) realloc ((*lst)->values,
            ((*lst)->length + _LIST_BLOCK) * sizeof (void *));

    if ((*lst)->values[(*lst)->length] != NULL)
        free ((*lst)->values[(*lst)->length]);
    (*lst)->values[(*lst)->length++] = value;
}

void
_list_delete (list ** lst,
        int pos)
{
    int i;
    if (!((*lst)->length > pos))
        return;

    for (i = pos; (pos + 1) < (*lst)->length; i++)
        (*lst)->values[i] = (*lst)->values[i + 1];

    (*lst)->length--;
}

void *
_list_get (list ** lst,
        int pos)
{
    if (!((*lst)->length > pos))
        return NULL;

    return (*lst)->values[pos];
}

void
_list_set (list ** lst,
        int pos,
        void * value)
{
    if (!((*lst)->length + (_LIST_BLOCK - ((*lst)->length % _LIST_BLOCK)) > pos))
        (*lst)->values = (void **) realloc ((*lst)->values,
                (pos + (_LIST_BLOCK - (pos % _LIST_BLOCK))) * sizeof (void *));

    if ((*lst)->values[pos] != NULL)
        free ((*lst)->values[pos]);
    (*lst)->values[pos] = value;
    (*lst)->length = pos + 1;
}

void
_list_free (list ** lst)
{
    int i;
    for (i = 0; i < ((*lst)->length + (_LIST_BLOCK - ((*lst)->length % _LIST_BLOCK))); i++)
        if ((*lst)->values[i] != NULL)
            free ((*lst)->values[i]);

    free (*lst);
}
