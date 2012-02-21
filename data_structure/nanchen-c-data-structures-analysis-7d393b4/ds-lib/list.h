/* START: fig3_6.txt */
#ifndef _List_H
#define _List_H
typedef void* ListElement;

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode ListPosition;

List List_makeEmpty(List L);
int List_isEmpty(List L);
int List_isLast(ListPosition P, List L);
ListPosition List_find(ListElement X, List L);
void List_delete(ListElement X, List L);
ListPosition List_findPrevious(ListElement X, List L);
void List_insert(ListElement X, List L, ListPosition P);
void List_deleteList(List L);
ListPosition List_header(List L);
ListPosition List_first(List L);
ListPosition List_last(List L);
ListPosition List_advance(ListPosition P);
ListElement List_retrieve(ListPosition P);

#endif    /* _List_H */
/* END */
