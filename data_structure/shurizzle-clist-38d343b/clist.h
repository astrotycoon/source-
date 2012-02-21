#ifndef _CLIST_H
#define _CLIST_H
#define _LIST_BLOCK 10

#define list_append(a,b)    _list_append (&(a), (void *)(&(b)))
#define list_delete(a,b)    _list_delete (&(a), b)
/* Don't care of int ** casting, any type's pointer's size is 1 byte :D */
#define list_get(a,b,c)     (* (int **)(&(c)) = * ((int **)_list_get (&(a), b)));
#define list_set(a,b,c)     _list_set (&(a), b, (void *)(&(c)))
#define list_free(a)        _list_free (&(a))

typedef struct list list;

struct list
{
    void ** values;
    int length;
};

extern  list    *   list_new        (void);
extern  void        _list_append    (list **, void *);
extern  void        _list_delete    (list **, int);
extern  void    *   _list_get       (list **, int);
extern  void        _list_set       (list **, int, void *);
extern  void        _list_free      (list **);

#endif
