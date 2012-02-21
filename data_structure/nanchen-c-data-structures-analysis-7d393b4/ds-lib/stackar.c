        #include "stackar.h"
        #include "fatal.h"
        #include <stdlib.h>

        #define EmptyTOS ( -1 )
        #define MinStackSize ( 5 )

        struct StackRecord
        {
            int Capacity;
            int TopOfStack;
            ElementType *Array;
        };

/* START: fig3_48.txt */
        int
        Stack_isEmpty( Stack S )
        {
            return S->TopOfStack == EmptyTOS;
        }
/* END */

        int
        Stack_isFull( Stack S )
        {
            return S->TopOfStack == S->Capacity - 1;
        }

/* START: fig3_46.txt */
        Stack
        Stack_createStack( int MaxElements )
        {
            Stack S;

/* 1*/      if( MaxElements < MinStackSize )
/* 2*/          error( "Stack size is too small" );

/* 3*/      S = malloc( sizeof( struct StackRecord ) );
/* 4*/      if( S == NULL )
/* 5*/          fatalError( "Out of space!!!" );

/* 6*/      S->Array = malloc( sizeof( ElementType ) * MaxElements );
/* 7*/      if( S->Array == NULL )
/* 8*/          fatalError( "Out of space!!!" );
/* 9*/      S->Capacity = MaxElements;
/*10*/      Stack_makeEmpty( S );

/*11*/      return S;
        }
/* END */

/* START: fig3_49.txt */
        void
        Stack_makeEmpty( Stack S )
        {
            S->TopOfStack = EmptyTOS;
        }
/* END */

/* START: fig3_47.txt */
        void
        Stack_disposeStack( Stack S )
        {
            if( S != NULL )
            {
                free( S->Array );
                free( S );
            }
        }
/* END */

/* START: fig3_50.txt */
        void
        Stack_push( ElementType X, Stack S )
        {
            if( Stack_isFull( S ) )
                error( "Full stack" );
            else
                S->Array[ ++S->TopOfStack ] = X;
        }
/* END */


/* START: fig3_51.txt */
        ElementType
        Stack_top( Stack S )
        {
            if( !Stack_isEmpty( S ) )
                return S->Array[ S->TopOfStack ];
            error( "Empty stack" );
            return 0;  /* Return value used to avoid warning */
        }
/* END */

/* START: fig3_52.txt */
        void
        Stack_pop( Stack S )
        {
            if( Stack_isEmpty( S ) )
                error( "Empty stack" );
            else
                S->TopOfStack--;
        }
/* END */

/* START: fig3_53.txt */
        ElementType
        Stack_topAndPop( Stack S )
        {
            if( !Stack_isEmpty( S ) )
                return S->Array[ S->TopOfStack-- ];
            error( "Empty stack" );
            return 0;  /* Return value used to avoid warning */
        }
/* END */
