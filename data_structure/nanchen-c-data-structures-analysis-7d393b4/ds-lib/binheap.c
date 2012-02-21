        #include "binheap.h"
        #include "fatal.h"
        #include <stdlib.h>

        #define MinPQSize (10)
        #define MinData (-32767)

        struct HeapStruct
        {
            int Capacity;
            int Size;
            ElementType *Elements;
        };

/* START: fig6_0.txt */
        PriorityQueue
        BinaryHeap_initialize( int MaxElements )
        {
            PriorityQueue H;

/* 1*/      if( MaxElements < MinPQSize )
/* 2*/          error( "Priority queue size is too small" );

/* 3*/      H = malloc( sizeof( struct HeapStruct ) );
/* 4*/      if( H ==NULL )
/* 5*/          fatalError( "Out of space!!!" );

            /* Allocate the array plus one extra for sentinel */
/* 6*/      H->Elements = malloc( ( MaxElements + 1 )
                                    * sizeof( ElementType ) );
/* 7*/      if( H->Elements == NULL )
/* 8*/          fatalError( "Out of space!!!" );

/* 9*/      H->Capacity = MaxElements;
/*10*/      H->Size = 0;
/*11*/      H->Elements[ 0 ] = MinData;

/*12*/      return H;
        }
/* END */

        void
        BinaryHeap_makeEmpty( PriorityQueue H )
        {
            H->Size = 0;
        }

/* START: fig6_8.txt */
        /* H->Element[ 0 ] is a sentinel */

        void
        BinaryHeap_insert( ElementType X, PriorityQueue H )
        {
            int i;

            if( BinaryHeap_isFull( H ) )
            {
                error( "Priority queue is full" );
                return;
            }

            for( i = ++H->Size; H->Elements[ i / 2 ] > X; i /= 2 )
                H->Elements[ i ] = H->Elements[ i / 2 ];
            H->Elements[ i ] = X;
        }
/* END */

/* START: fig6_12.txt */
        ElementType
        BinaryHeap_deleteMin( PriorityQueue H )
        {
            int i, Child;
            ElementType MinElement, LastElement;

/* 1*/      if( BinaryHeap_isEmpty( H ) )
            {
/* 2*/          error( "Priority queue is empty" );
/* 3*/          return H->Elements[ 0 ];
            }
/* 4*/      MinElement = H->Elements[ 1 ];
/* 5*/      LastElement = H->Elements[ H->Size-- ];

/* 6*/      for( i = 1; i * 2 <= H->Size; i = Child )
            {
                /* find smaller child */
/* 7*/          Child = i * 2;
/* 8*/          if( Child != H->Size && H->Elements[ Child + 1 ]
/* 9*/                                < H->Elements[ Child ] )
/*10*/              Child++;

                /* Percolate one level */
/*11*/          if( LastElement > H->Elements[ Child ] )
/*12*/              H->Elements[ i ] = H->Elements[ Child ];
                else
/*13*/              break;
            }
/*14*/      H->Elements[ i ] = LastElement;
/*15*/      return MinElement;
        }
/* END */

        ElementType
        BinaryHeap_findMin( PriorityQueue H )
        {
            if( !BinaryHeap_isEmpty( H ) )
                return H->Elements[ 1 ];
            error( "Priority Queue is Empty" );
            return H->Elements[ 0 ];
        }

        int
        BinaryHeap_isEmpty( PriorityQueue H )
        {
            return H->Size == 0;
        }

        int
        BinaryHeap_isFull( PriorityQueue H )
        {
            return H->Size == H->Capacity;
        }

        void
        BinaryHeap_destroy( PriorityQueue H )
        {
            free( H->Elements );
            free( H );
        }

        #if 0
/* START: fig6_14.txt */
        for( i = N / 2; i > 0; i-- )
            PercolateDown( i );
/* END */
        #endif
