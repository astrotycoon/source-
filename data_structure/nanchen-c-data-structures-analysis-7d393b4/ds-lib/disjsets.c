        /* Disjoint set data structure */
        /* All in one file because it's so short */

#define FastAlg

#define NumSets 128

/* START: fig8_6.txt */
        #ifndef _DisjSet_H

        typedef int DisjSet[ NumSets + 1 ];
        typedef int SetType;
        typedef int ElementType;

        void DisjointSet_initialize( DisjSet S );
        void DisjointSet_setUnion( DisjSet S, SetType Root1, SetType Root2 );
        SetType DisjointSet_find( ElementType X, DisjSet S );

        #endif  /* _DisjSet_H */
/* END */

/* START: fig8_7.txt */
        void
        DisjointSet_initialize( DisjSet S )
        {
            int i;

            for( i = NumSets; i > 0; i-- )
                S[ i ] = 0;
        }
/* END */

#ifdef SlowAlg
/* START: fig8_8.txt */
        /* Assumes Root1 and Root2 are roots */
        /* union is a C keyword, so this routine */
        /* is named DisjointSet_setUnion */

        void
        DisjointSet_setUnion( DisjSet S, SetType Root1, SetType Root2 )
        {
            S[ Root2 ] = Root1;
        }
/* END */

/* START: fig8_9.txt */
        SetType
        DisjointSet_find( ElementType X, DisjSet S )
        {
            if( S[ X ] <= 0 )
                return X;
            else
                return DisjointSet_find( S[ X ], S );
        }
/* END */
#else

/* START: fig8_13.txt */
        /* Assume Root1 and Root2 are roots */
        /* union is a C keyword, so this routine */
        /* is named DisjointSet_setUnion */

        void
        DisjointSet_setUnion( DisjSet S, SetType Root1, SetType Root2 )
        {
            if( S[ Root2 ] < S[ Root1 ] ) /* Root2 is deeper set */
                S[ Root1 ] = Root2;  /* Make Root2 new root */
            else
            {
                if( S[ Root1 ] == S[ Root2 ] )  /* Same height, */
                    S[ Root1 ]--;               /* so update */
                S[ Root2 ] = Root1;
            }
        }
/* END */

/* START: fig8_15.txt */
        SetType
        DisjointSet_find( ElementType X, DisjSet S )
        {
            if( S[ X ] <= 0 )
                return X;
            else
                return S[ X ] = DisjointSet_find( S[ X ], S );
        }
/* END */
#endif

DisjointSet_test( )
{
    DisjSet S;
    int i, j, k, Set1, Set2;

    DisjointSet_initialize( S );
    j = k = 1;
    while( k <= 8 )
    {
        j = 1;
        while( j < NumSets )
        {
            Set1 = DisjointSet_find( j, S );
            Set2 = DisjointSet_find( j + k, S );
            DisjointSet_setUnion( S, Set1, Set2 );
            j += 2 * k;
        }
        k *= 2;
    }
    i = 1;
    for( i = 1; i <= NumSets; i++ )
    {
        Set1 = DisjointSet_find( i, S );
        printf( "%d**", Set1 );
    }
    printf( "\n" );
    return 0;
}

