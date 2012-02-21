#include <stdio.h>
#include "queue.h"

int Queue_test(void)
{
    Queue Q;
    int i;

    Q = Queue_createQueue( 12 );

    for( i = 0; i < 10; i++ )
        Queue_enqueue( i, Q );

    while( !Queue_isEmpty( Q ) )
    {
        printf( "%d\n", Queue_front( Q ) );
        Queue_dequeue( Q );
    }
    for( i = 0; i < 10; i++ )
        Queue_enqueue( i, Q );

    while( !Queue_isEmpty( Q ) )
    {
        printf( "%d\n", Queue_front( Q ) );
        Queue_dequeue( Q );
    }

    Queue_disposeQueue( Q );
    return 0;
}
