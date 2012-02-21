#include "treap.h"
#include <stdio.h>

#define NumItems 12000

Treap_test( )
{

    Treap T;
    Position P;
    int i;
    int j = 0;

    T = Treap_initialize( );
    T = Treap_makeEmpty( NullNode );
    for( i = 0; i < NumItems; i++, j = ( j + 7 ) % NumItems )
        T = Treap_insert( j, T );
    for( i = 0; i < NumItems; i++ )
        if( ( P = Treap_find( i, T ) ) == NullNode || Treap_retrieve( P ) != i )
            printf( "Error1 at %d\n", i );

    for( i = 0; i < NumItems; i += 2 )
        T = Treap_remove( i, T );

    for( i = 1; i < NumItems; i += 2 )
        if( ( P = Treap_find( i, T ) ) == NullNode || Treap_retrieve( P ) != i )
           printf( "Error2 at %d\n", i );

    for( i = 0; i < NumItems; i += 2 )
        if( ( P = Treap_find( i, T ) ) != NullNode )
            printf( "Error3 at %d\n", i );
    printf( "Min is %d, Max is %d\n", Treap_retrieve( Treap_findMin( T ) ),
               Treap_retrieve( Treap_findMax( T ) ) );

    return 0;
}
