        #include "list.h"
        #include <stdlib.h>
        #include "fatal.h"
        #include "resourcetrack.h"

        /* Place in the interface file */
        struct Node
        {
            ListElement Element;
            ListPosition    Next;
        };

        const int SPACE_LIST = sizeof(List);
        const int SPACE_LIST_POS = sizeof(ListPosition);

        List
        List_makeEmpty( List L )
        {
            Resource_logSpace(SPACE_LIST);
            if( L != NULL ) // 1
                List_deleteList( L );
            L = malloc( sizeof( struct Node ) ); // 4
            Resource_logSpace(sizeof( struct Node ));
            if( L == NULL ){ // 1
                fatalError( "Out of memory!" );
                Resource_logTime(1);
            }
            L->Next = NULL; // 2
            Resource_logTime(10);
            Resource_logSpace(-SPACE_LIST);
            return L;
        }

/* START: fig3_8.txt */
        /* Return true if L is empty */

        int
        List_isEmpty( List L )
        {
            Resource_logSpace(SPACE_LIST);
            Resource_logTime(4);
            Resource_logSpace(-SPACE_LIST);
            return L->Next == NULL; //3
        }
/* END */

/* START: fig3_9.txt */
        /* Return true if P is the last position in list L */
        /* Parameter L is unused in this implementation */

        int List_isLast( ListPosition P, List L )
        {
            const int SPACE = sizeof(List) + sizeof(ListPosition);
            Resource_logSpace(SPACE);
            Resource_logTime(4);
            Resource_logSpace(-SPACE);
            return P->Next == NULL;
        }
/* END */

/* START: fig3_10.txt */
        /* Return ListPosition of X in L; NULL if not found */

        ListPosition
        List_find( ListElement X, List L )
        {
            const int SPACE = sizeof(ListElement) + sizeof(List);
            Resource_logSpace(SPACE);
            ListPosition P; //1

/* 1*/      P = L->Next; //2
/* 2*/      while( P != NULL && P->Element != X ){ //4
/* 3*/          P = P->Next; //2
                Resource_logTime(2);
            }
            Resource_logTime(9);
            Resource_logSpace(-SPACE);

/* 4*/      return P; //1
        }
/* END */

/* START: fig3_11.txt */
        /* List_delete from a list */
        /* Cell pointed to by P->Next is wiped out */
        /* Assume that the position is legal */
        /* Assume use of a header node */

        void
        List_delete( ListElement X, List L )
        {
            const int SPACE = sizeof(ListElement) + sizeof(List);
            Resource_logSpace(SPACE);
            ListPosition P, TmpCell; //2

            P = List_findPrevious( X, L ); //1

            if( !List_isLast( P, L ) )  /* Assumption of header use */ //2
            {                      /* X is found; delete it */
                TmpCell = P->Next; //2
                P->Next = TmpCell->Next;  /* Bypass deleted cell */ //3
                free( TmpCell ); //1
                Resource_logSpace(-sizeof(struct Node));
                Resource_logTime(6);
            }
            Resource_logTime(7);
            Resource_logSpace(-SPACE);
        }
/* END */

/* START: fig3_12.txt */
        /* If X is not found, then Next field of returned value is NULL */
        /* Assumes a header */

        ListPosition
        List_findPrevious( ListElement X, List L )
        {
            const int SPACE = sizeof(ListElement) + sizeof(List);
            Resource_logSpace(SPACE);
            ListPosition P; //1

/* 1*/      P = L; //1
/* 2*/      while( P->Next != NULL && P->Next->Element != X ){ //6
/* 3*/          P = P->Next; //2
                Resource_logTime(2);
            }
            Resource_logTime(10);
            Resource_logSpace(-SPACE);
/* 4*/      return P; //1
        }
/* END */

/* START: fig3_13.txt */
        /* List_insert (after legal position P) */
        /* List_header implementation assumed */
        /* Parameter L is unused in this implementation */

        void
        List_insert( ListElement X, List L, ListPosition P )
        {
            const int SPACE = sizeof(ListElement) + sizeof(List) + sizeof(ListPosition);
            Resource_logSpace(SPACE);
            ListPosition TmpCell; //1

/* 1*/      TmpCell = malloc( sizeof( struct Node ) ); //3
            Resource_logSpace(sizeof(struct Node));
/* 2*/      if( TmpCell == NULL ) //1
/* 3*/          fatalError( "Out of space!!!" );

/* 4*/      TmpCell->Element = X; //2
/* 5*/      TmpCell->Next = P->Next; //3
/* 6*/      P->Next = TmpCell; //2
            Resource_logTime(14);
            Resource_logSpace(-SPACE);
        }
/* END */

#if 0
/* START: fig3_14.txt */
        /* Incorrect List_deleteList algorithm */

        void
        List_deleteList( List L )
        {
            Resource_logSpace(SPACE_LIST);
            ListPosition P;

/* 1*/      P = L->Next;  /* List_header assumed */
/* 2*/      L->Next = NULL;
/* 3*/      while( P != NULL )
            {
/* 4*/          free( P );
/* 5*/          P = P->Next;
            }
            Resource_logSpace(-SPACE_LIST);
        }
/* END */
#endif

/* START: fig3_15.txt */
        /* Correct List_deleteList algorithm */

        void
        List_deleteList( List L )
        {
            Resource_logSpace(SPACE_LIST);
            ListPosition P, Tmp; //2

/* 1*/      P = L->Next;  /* List_header assumed */ // 2
/* 2*/      L->Next = NULL; //2
/* 3*/      while( P != NULL ) //1
            {
/* 4*/          Tmp = P->Next; //2
/* 5*/          free( P );  //1
/* 6*/          P = Tmp; // 1
                Resource_logTime(4);
//                printf("List_deleteList(), sizeof(P) = %d\n", sizeof(P));
                Resource_logSpace(-sizeof(struct Node));
            }
            Resource_logTime(9);
            Resource_logSpace(-SPACE_LIST);
        }
/* END */

        ListPosition
        List_header( List L )
        {
            Resource_logSpace(SPACE_LIST);
            Resource_logTime(2);
            return L;
            Resource_logSpace(-SPACE_LIST);
        }

        ListPosition
        List_first( List L )
        {
            Resource_logSpace(SPACE_LIST);
            Resource_logTime(3);
            return L->Next;
            Resource_logSpace(-SPACE_LIST);
        }

        ListPosition List_last(List L){
            Resource_logSpace(SPACE_LIST);
            ListPosition P; //1

            P = L->Next; //2
            while( P->Next != NULL ){ //4
                P = P->Next; //2
                Resource_logTime(2);
            }
            Resource_logTime(9);
            Resource_logSpace(-SPACE_LIST);
            return P; //1
        }

        ListPosition
        List_advance( ListPosition P )
        {
            Resource_logSpace(SPACE_LIST_POS);
            Resource_logTime(3);
            Resource_logSpace(-SPACE_LIST_POS);
            return P->Next;
        }

        ListElement
        List_retrieve( ListPosition P )
        {
            Resource_logSpace(SPACE_LIST_POS);
            Resource_logTime(3);
            Resource_logSpace(-SPACE_LIST_POS);
            return P->Element;
        }
