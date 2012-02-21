CLIST
=====
clist is a simple library which manage a list of variables independently from
their types.

THE TYPE
--------
It's defined a type, list, it must be used as a pointer (list *) to work
correctly

LIST AND EXPLAINATION OF FUNCTIONS
----------------------------------

    list * list_new (void);

Simply initialize a list.

RETURN value: a list pointer

-------------

    void list_append (list * lst, anytype value);
is an alias of
    void _list_append (list ** lst, void * value);

lst:    is the list pointer
value:  is the value to insert

Append an element to list

RETURN value: nothing

-------------

    void list_delete (list * lst, int position);
is an alias of
    void _list_delete (list ** lst, int position);

lst:        is the list pointer
position:   is the position of element to delete

Delete a value from list and shift the following

RETURN value: nothing

-------------

    void list_get (list * lst, int position, anytype var);
is an alias of
    (* (int **) (&var) = *((int **) _list_get (list ** lst, int
                position)))

lst:        is the list pointer
position:   is the position of element to get
var:        is the variable which will contain the value

Get a value from list

RETURN value: nothing (in the alias)

-------------

    void list_set (list * lst, int position, anytype value);
is an alias of
    void _list_set (list ** lst, int position, void * value);

lst:        is the list pointer
position:   is the position of element to set
value:      is the value to insert

Set a value in list

RETURN VALUE: nothing

-------------

    void list_free (list * lst);
is an alias of
    void _list_free (list ** lst);

lst:        is the list pointer

Free the space allocated by the list

RETURN VALUE: nothing

-------------

THAT'S ALL, ENJOY :D
