        #include "stackli.h"
        #include "fatal.h"
        #include <stdlib.h>

        struct Node
        {
            ElementType Element;
            PtrToNode   Next;
        };

/* START: fig3_40.txt */
        int
        Stack_isEmpty( Stack S )
        {
            return S->Next == NULL;
        }
/* END */

/* START: fig3_41.txt */
        Stack
        Stack_createStack( void )
        {
            Stack S;

            S = malloc( sizeof( struct Node ) );
            if( S == NULL )
                FatalError( "Out of space!!!" );
            S->Next = NULL;
            Stack_makeEmpty( S );
            return S;
        }

        void
        Stack_makeEmpty( Stack S )
        {
            if( S == NULL )
                Error( "Must use CreateStack first" );
            else
                while( !Stack_isEmpty( S ) )
                    Stack_pop( S );
        }
/* END */

        void
        Stack_disposeStack( Stack S )
        {
            Stack_makeEmpty( S );
            free( S );
        }

/* START: fig3_42.txt */
        void
        Stack_push( ElementType X, Stack S )
        {
            PtrToNode TmpCell;

            TmpCell = malloc( sizeof( struct Node ) );
            if( TmpCell == NULL )
                FatalError( "Out of space!!!" );
            else
            {
                TmpCell->Element = X;
                TmpCell->Next = S->Next;
                S->Next = TmpCell;
            }
        }
/* END */

/* START: fig3_43.txt */
        ElementType
        Stack_top( Stack S )
        {
            if( !Stack_isEmpty( S ) )
                return S->Next->Element;
            Error( "Empty stack" );
            return 0;  /* Return value used to avoid warning */
        }
/* END */

/* START: fig3_44.txt */
        void
        Stack_pop( Stack S )
        {
            PtrToNode FirstCell;

            if( Stack_isEmpty( S ) )
                Error( "Empty stack" );
            else
            {
                FirstCell = S->Next;
                S->Next = S->Next->Next;
                free( FirstCell );
            }
        }
/* END */
