#include "pairheap.h"
#include <stdio.h>

void
sleep( int x )
{
    int i, j, k, m;

    for( i = 0; i < 10000; i++ )
        for( j = 0; j < 1000; j++ )
            for( k = 0; k < x; k++ )
                m++;
    printf( "Done sleeping!! %d", m );
}

#define MaxSize 500

PairHeap_test( )
{
    PairHeap H;
    Position P[ MaxSize ];
    int i, j;
    int AnItem;

    H = PairHeap_initialize( );
    for( i=0, j=MaxSize/2; i<MaxSize; i++, j=( j+71)%MaxSize )
        H = PairHeap_insert( j + MaxSize, H, &P[ j ] );

    printf( "Done inserting\n" );

    for( i = 0, j = MaxSize / 2; i <MaxSize; i++, j=(j+51)%MaxSize )
        H = PairHeap_decreaseKey( P[ j ], MaxSize, H );

    j = 0;
    while( !PairHeap_isEmpty( H ) )
    {
        if( ( ( H = PairHeap_deleteMin( &AnItem, H ) ), AnItem ) != j++ )
            printf( "Error in DeleteMin, %d\n", j );
    }
    printf( "Done...\n" );

    return 0;
}
