#define SepChain    /* Define the appropriate hash algorithm */

#ifdef SepChain
    #include "hashsep.h"
#endif

#ifdef QuadProb
    #include "hashquad.h"
#endif


#include <stdio.h>

#define NumItems 400

Hash_test( )
{
    HashTable H;
    Position P;
    int i;
    int j = 0;
    int CurrentSize;

    H = Hash_initializeTable( CurrentSize = 13 );

    for( i = 0; i < NumItems; i++, j += 71 )
    {
    #ifdef QuadProb
        if( i > CurrentSize / 2 )
        {
            H = Hash_rehash( H );
            printf( "Rehashing...\n" );
            CurrentSize *= 2;
        }
    #endif
        Hash_insert( j, H );
    }

    for( i = 0, j = 0; i < NumItems; i++, j += 71 )
    #ifdef SepChain
        if( ( P = Hash_find( j, H ) ) == NULL || Hash_retrieve( P ) != j )
    #endif
    #ifdef QuadProb
        if( Hash_retrieve( ( P = Hash_find( j, H ) ), H ) != j )
    #endif
            printf( "Error at %d\n", j );

    printf( "End of program.\n" );
    return 0;
}
