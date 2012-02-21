#include "aatree.h"
#include <stdio.h>

#define NumItems 20

int AATree_test(void){
    AATree T;
    Position P;
    int i;
    int j = 0;

    T = AATree_initialize( );
    T = AATree_makeEmpty( NullNode );
    for( i = 0; i < NumItems; i++, j = ( j + 7 ) % NumItems )
        T = AATree_insert( j, T );
    for( i = 0; i < NumItems; i++ )
        if( ( P = AATree_find( i, T ) ) == NullNode || AATree_retrieve( P ) != i )
            printf( "Error at %d\n", i );

    for( i = 0; i < NumItems; i += 2 )
        T = AATree_remove( i, T );

    for( i = 1; i < NumItems; i += 2 )
        if( ( P = AATree_find( i, T ) ) == NullNode || AATree_retrieve( P ) != i )
            printf( "Error at %d\n", i );

    for( i = 0; i < NumItems; i += 2 )
        if( ( P = AATree_find( i, T ) ) != NullNode )
            printf( "Error at %d\n", i );

    printf( "Min is %d, Max is %d\n", AATree_retrieve( AATree_findMin( T ) ),
               AATree_retrieve( AATree_findMax( T ) ) );

    return 0;
}
