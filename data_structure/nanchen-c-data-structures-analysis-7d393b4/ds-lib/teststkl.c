#include <stdio.h>
#include "stackli.h"

main( )
{
    Stack S;
    int i;

    S = Stack_createStack( );
    for( i = 0; i < 10; i++ )
        Stack_push( i, S );

    while( !Stack_isEmpty( S ) )
    {
        printf( "%d\n", Stack_top( S ) );
        Stack_pop( S );
    }

    Stack_disposeStack( S );
    return 0;
}
