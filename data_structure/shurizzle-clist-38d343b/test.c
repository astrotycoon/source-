#include    <stdio.h>
#include    <stdlib.h>

#include    "clist.h"

int
main (void)
{
    list * test = list_new ();
    int i, * tmp, penis;
    for (i = 0; i < 15; i++)
    {
        tmp = (int *) calloc (1, sizeof (int));
        *tmp = i;
        list_append (test, *tmp);
    }

    printf ("length: %d\n", test->length);

    tmp = (int *) calloc (1, sizeof (int));
    *tmp = 69,
    list_set (test, 127, *tmp);

    for (i = 0; i < 15; i++)
    {
        list_get (test, i, penis);
        printf ("element: %d\tvalue: %d\n", i, penis);
    }

    list_get (test, 127, penis);
    printf ("element: 127\tvalue: %d\n", penis);

    list_free (test);

    return 0;
}
