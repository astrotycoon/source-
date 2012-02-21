        #include "listcursor.h"
        #include <stdlib.h>
        #include "fatal.h"

        /* Place in the interface file */
        struct Node
        {
            ElementType Element;
            Position    Next;
        };

        struct Node CursorSpace[ SpaceSize ];

/* START: fig3_31.txt */
        static Position
        CursorAlloc( void )
        {
            Position P;

            P = CursorSpace[ 0 ].Next;
            CursorSpace[ 0 ].Next = CursorSpace[ P ].Next;

            return P;
        }

        static void
        CursorFree( Position P )
        {
            CursorSpace[ P ].Next = CursorSpace[ 0 ].Next;
            CursorSpace[ 0 ].Next = P;
        }
/* END */

        void
        ListCursor_initializeCursorSpace( void )
        {
            int i;

            for( i = 0; i < SpaceSize; i++ )
                CursorSpace[ i ].Next = i + 1;
            CursorSpace[ SpaceSize - 1 ].Next = 0;
        }

        List
        ListCursor_MakeEmpty( List L )
        {
            if( L != NULL )
                ListCursor_deleteList( L );
            L = CursorAlloc( );
            if( L == 0 )
                fatalError( "Out of memory!" );
            CursorSpace[ L ].Next = 0;
            return L;
        }

/* START: fig3_32.txt */
        /* Return true if L is empty */

        int
        ListCursor_isEmpty( List L )
        {
            return CursorSpace[ L ].Next == 0;
        }
/* END */

/* START: fig3_33.txt */
        /* Return true if P is the last position in list L */
        /* Parameter L is unused in this implementation */

        int ListCursor_isLast( Position P, List L )
        {
            return CursorSpace[ P ].Next == 0;
        }
/* END */

/* START: fig3_34.txt */
        /* Return Position of X in L; 0 if not found */
        /* Uses a header node */

        Position
        ListCursor_find( ElementType X, List L )
        {
            Position P;

/* 1*/      P = CursorSpace[ L ].Next;
/* 2*/      while( P && CursorSpace[ P ].Element != X )
/* 3*/          P = CursorSpace[ P ].Next;

/* 4*/      return P;
        }
/* END */

/* START: fig3_35.txt */
        /* ListCursor_delete from a list */
        /* Assume that the position is legal */
        /* Assume use of a header node */

        void
        ListCursor_delete( ElementType X, List L )
        {
            Position P, TmpCell;

            P = ListCursor_findPrevious( X, L );

            if( !ListCursor_isLast( P, L ) )  /* Assumption of header use */
            {                      /* X is found; delete it */
                TmpCell = CursorSpace[ P ].Next;
                CursorSpace[ P ].Next = CursorSpace[ TmpCell ].Next;
                CursorFree( TmpCell );
            }
        }
/* END */

        /* If X is not found, then Next field of returned value is 0 */
        /* Assumes a header */

        Position
        ListCursor_findPrevious( ElementType X, List L )
        {
            Position P;

/* 1*/      P = L;
/* 2*/      while( CursorSpace[ P ].Next &&
                    CursorSpace[ CursorSpace[ P ].Next ].Element != X )
/* 3*/          P = CursorSpace[ P ].Next;

/* 4*/      return P;
        }

/* START: fig3_36.txt */
        /* ListCursor_insert (after legal position P) */
        /* ListCursor_header implementation assumed */
        /* Parameter L is unused in this implementation */

        void
        ListCursor_insert( ElementType X, List L, Position P )
        {
            Position TmpCell;

/* 1*/      TmpCell = CursorAlloc( );
/* 2*/      if( TmpCell == 0 )
/* 3*/          fatalError( "Out of space!!!" );

/* 4*/      CursorSpace[ TmpCell ].Element = X;
/* 5*/      CursorSpace[ TmpCell ].Next = CursorSpace[ P ].Next;
/* 6*/      CursorSpace[ P ].Next = TmpCell;
        }
/* END */


        /* Correct ListCursor_deleteList algorithm */

        void
        ListCursor_deleteList( List L )
        {
            Position P, Tmp;

/* 1*/      P = CursorSpace[ L ].Next;  /* ListCursor_header assumed */
/* 2*/      CursorSpace[ L ].Next = 0;
/* 3*/      while( P != 0 )
            {
/* 4*/          Tmp = CursorSpace[ P ].Next;
/* 5*/          CursorFree( P );
/* 6*/          P = Tmp;
            }
        }

        Position
        ListCursor_header( List L )
        {
            return L;
        }

        Position
        ListCursor_first( List L )
        {
            return CursorSpace[ L ].Next;
        }

        Position
        ListCursor_advance( Position P )
        {
            return CursorSpace[ P ].Next;
        }

        ElementType
        ListCursor_retrieve( Position P )
        {
            return CursorSpace[ P ].Element;
        }
