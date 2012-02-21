#include "leftheap.h"
#include <stdio.h>

#define MaxSize 5000

LeftistHeap_test( )
{
    PriorityQueue H;
    int i, j;

    H = LeftistHeap_initialize( );
    for( i=0, j=MaxSize/2; i<MaxSize; i++, j=( j+17)%MaxSize )
        Insert( j, H );

    j = 0;
    while( !LeftistHeap_isEmpty( H ) )
        if( LeftistHeap_findMin( H ) != j++ )
            printf( "Error in DeleteMin, %d\n", j );
        else
            H = LeftistHeap_deleteMin1( H );
    printf( "Done...\n" );
    return 0;
}
