#include "dsl.h"
#include <stdio.h>

#define N 800

int DSList_main(void) {
    SkipList T;
    Position P;
    int i;
    int j = 0;

    T = DSList_initialize( );
    T = DSList_makeEmpty( T );

    for( i = 0; i < N; i++, j = ( j + 7 ) % N )
        T = DSList_insert( j, T );
    printf( "Inserts are complete\n" );


    for( i = 0; i < N; i++ )
        if( ( P = DSList_find( i, T ) ) == NULL || DSList_retrieve( P ) != i )
            printf( "Error at %d\n", i );

    printf( "Min is %d, Max is %d\n", DSList_retrieve( DSList_findMin( T ) ),
               DSList_retrieve( DSList_findMax( T ) ) );

    return 0;
}
