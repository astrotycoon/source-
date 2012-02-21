#include "splay.h"
#include <stdio.h>
#define NumItems 500

SplayTree_test( )
{
    SplayTree T;
    SplayTree P;
    int i;
    int j = 0;

    T = SplayTree_initialize( );
    T = SplayTree_makeEmpty( T );
    for( i = 0; i < NumItems; i++, j = ( j + 7 ) % NumItems )
        T = SplayTree_insert( j, T );
  
    for( j = 0; j < 2; j++ )
        for( i = 0; i < NumItems; i++ )
        {
            T = SplayTree_find( i, T );
            if( SplayTree_retrieve( T ) != i )
                printf( "Error1 at %d\n", i );
        }

    printf( "Entering remove\n" );

    for( i = 0; i < NumItems; i += 2 )
        T = SplayTree_remove( i, T );

    for( i = 1; i < NumItems; i += 2 )
    {
        T = SplayTree_find( i, T );
        if( SplayTree_retrieve( T ) != i )
            printf( "Error2 at %d\n", i );
    }

    for( i = 0; i < NumItems; i += 2 )
    {
        T = SplayTree_find( i, T );
        if( SplayTree_retrieve( T ) == i )
            printf( "Error3 at %d\n", i );
    }

    printf( "Min is %d\n", SplayTree_retrieve( T = SplayTree_findMin( T ) ) );
    printf( "Max is %d\n", SplayTree_retrieve( T = SplayTree_findMax( T ) ) );

    return 0;
}
