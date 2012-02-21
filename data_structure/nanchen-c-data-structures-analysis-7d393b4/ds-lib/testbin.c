#include "binomial.h"
#include <stdio.h>

#define MaxSize (12000)

Binomial_test()
{
    BinQueue H;
    int i, j;
    ElementType AnItem;

    H = BinaryHeap_initialize( );
    for( i=0, j=MaxSize/2; i<MaxSize; i++, j=( j+71)%MaxSize )
    {
 /*     printf( "Inserting %d\n", j );
 */     H = BinaryHeap_insert( j, H );
    }
 #if 1
    j = 0;
    while( !BinaryHeap_isEmpty( H ) )
    {
 /*     printf( "BinaryHeap_deleteMin\n" );
        H = BinaryHeap_deleteMin( &AnItem, H );
  */    if( BinaryHeap_deleteMin( H ) != j++ )
            printf( "Error in DeleteMin, %d\n", j );
    }
    if( j != MaxSize )
        printf( "Error in counting\n" );
    #endif
    printf( "Done...\n" );
    return 0;
}
