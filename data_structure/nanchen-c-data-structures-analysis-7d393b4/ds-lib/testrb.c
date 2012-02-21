#include "redblack.h"
#include <stdio.h>

#define N 800

RedBlackTree_test( )
{
    RedBlackTree T;
    Position P;
    int i;
    int j = 0;

    T = RedBlackTree_initialize( );
    T = RedBlackTree_makeEmpty( T );

    for( i = 0; i < N; i++, j = ( j + 7 ) % N )
        T = RedBlackTree_insert( j, T );
    printf( "Inserts are complete\n" );



    for( i = 0; i < N; i++ )
        if( ( P = RedBlackTree_find( i, T ) ) == NULL || RedBlackTree_retrieve( P ) != i )
            printf( "Error at %d\n", i );


    printf( "Min is %d, Max is %d\n", RedBlackTree_retrieve( RedBlackTree_findMin( T ) ),
               RedBlackTree_retrieve( RedBlackTree_findMax( T ) ) );

    return 0;
}
