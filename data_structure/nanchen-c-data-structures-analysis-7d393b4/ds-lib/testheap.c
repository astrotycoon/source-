#include "binheap.h"
#include <stdio.h>

#define MaxSize (1000)

BinaryHeap_test()
{
    PriorityQueue H;
    int i, j;

    H = BinaryHeap_initialize( MaxSize );
    for( i=0, j=MaxSize/2; i<MaxSize; i++, j=( j+71)%MaxSize )
        BinaryHeap_insert( j, H );

    j = 0;
    while( !BinaryHeap_isEmpty( H ) )
        if( BinaryHeap_deleteMin( H ) != j++ )
            printf( "Error in DeleteMin, %d\n", j );
    printf( "Done...\n" );
    return 0;
}
