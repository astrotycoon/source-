        #include "binomial.h"
        #include "fatal.h"


/* START: fig6_52.txt */
        typedef struct BinNode *Position;

        struct BinNode
        {
            ElementType Element;
            Position    LeftChild;
            Position    NextSibling;
        };

        struct Collection
        {
            int CurrentSize;
            BinTree TheTrees[ MaxTrees ];
        };

        BinQueue
        BinomialQueue_initialize( void )
        {
            BinQueue H;
            int i;

            H = malloc( sizeof( struct Collection ) );
            if( H == NULL )
                fatalError( "Out of space!!!" );
            H->CurrentSize = 0;
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            return H;
        }

        static void
        DestroyTree( BinTree T )
        {
            if( T != NULL )
            {
                DestroyTree( T->LeftChild );
                DestroyTree( T->NextSibling );
                free( T );
            }
        }

        void
        BinomialQueue_destroy( BinQueue H )
        {
            int i;

            for( i = 0; i < MaxTrees; i++ )
                DestroyTree( H->TheTrees[ i ] );
        }

        BinQueue
        BinomialQueue_makeEmpty( BinQueue H )
        {
            int i;

            BinomialQueue_destroy( H );
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            H->CurrentSize = 0;

            return H;
        }

        /* Not optimized for O(1) amortized performance */
        BinQueue
        BinomialQueue_insert( ElementType Item, BinQueue H )
        {
            BinTree NewNode;
            BinQueue OneItem;

            NewNode = malloc( sizeof( struct BinNode ) );
            if( NewNode == NULL )
                fatalError( "Out of space!!!" );
            NewNode->LeftChild = NewNode->NextSibling = NULL;
            NewNode->Element = Item;

            OneItem = BinomialQueue_initialize( );
            OneItem->CurrentSize = 1;
            OneItem->TheTrees[ 0 ] = NewNode;

            return BinomialQueue_merge( H, OneItem );
        }

/* START: fig6_56.txt */
        ElementType
        BinomialQueue_deleteMin( BinQueue H )
        {
            int i, j;
            int MinTree;   /* The tree with the minimum item */
            BinQueue DeletedQueue;
            Position DeletedTree, OldRoot;
            ElementType MinItem;

            if( BinomialQueue_isEmpty( H ) )
            {
                error( "Empty binomial queue" );
                return -Infinity;
            }

            MinItem = Infinity;
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                    H->TheTrees[ i ]->Element < MinItem )
                {
                    /* Update minimum */
                    MinItem = H->TheTrees[ i ]->Element;
                    MinTree = i;
                }
            }

            DeletedTree = H->TheTrees[ MinTree ];
            OldRoot = DeletedTree;
            DeletedTree = DeletedTree->LeftChild;
            free( OldRoot );

            DeletedQueue = BinomialQueue_initialize( );
            DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
            for( j = MinTree - 1; j >= 0; j-- )
            {
                DeletedQueue->TheTrees[ j ] = DeletedTree;
                DeletedTree = DeletedTree->NextSibling;
                DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
            }

            H->TheTrees[ MinTree ] = NULL;
            H->CurrentSize -= DeletedQueue->CurrentSize + 1;

            BinomialQueue_merge( H, DeletedQueue );
            return MinItem;
        }
/* END */

        ElementType
        BinomialQueue_findMin( BinQueue H )
        {
            int i;
            ElementType MinItem;

            if( BinomialQueue_isEmpty( H ) )
            {
                error( "Empty binomial queue" );
                return 0;
            }

            MinItem = Infinity;
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                            H->TheTrees[ i ]->Element < MinItem )
                    MinItem = H->TheTrees[ i ]->Element;
            }

            return MinItem;
        }

        int
        BinomialQueue_isEmpty( BinQueue H )
        {
            return H->CurrentSize == 0;
        }

        int BinomialQueue_isFull( BinQueue H )
        {
            return H->CurrentSize == Capacity;
        }

/* START: fig6_54.txt */
        /* Return the result of merging equal-sized T1 and T2 */
        BinTree
        CombineTrees( BinTree T1, BinTree T2 )
        {
            if( T1->Element > T2->Element )
                return CombineTrees( T2, T1 );
            T2->NextSibling = T1->LeftChild;
            T1->LeftChild = T2;
            return T1;
        }
/* END */

/* START: fig6_55.txt */
        /* BinomialQueue_merge two binomial queues */
        /* Not optimized for early termination */
        /* H1 contains merged result */

        BinQueue
        BinomialQueue_merge( BinQueue H1, BinQueue H2 )
        {
            BinTree T1, T2, Carry = NULL;
            int i, j;

            if( H1->CurrentSize + H2->CurrentSize > Capacity )
                error( "Merge would exceed capacity" );

            H1->CurrentSize += H2->CurrentSize;
            for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
            {
                T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

                switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
                {
                    case 0: /* No trees */
                    case 1: /* Only H1 */
                        break;
                    case 2: /* Only H2 */
                        H1->TheTrees[ i ] = T2;
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 4: /* Only Carry */
                        H1->TheTrees[ i ] = Carry;
                        Carry = NULL;
                        break;
                    case 3: /* H1 and H2 */
                        Carry = CombineTrees( T1, T2 );
                        H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                        break;
                    case 5: /* H1 and Carry */
                        Carry = CombineTrees( T1, Carry );
                        H1->TheTrees[ i ] = NULL;
                        break;
                    case 6: /* H2 and Carry */
                        Carry = CombineTrees( T2, Carry );
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 7: /* All three */
                        H1->TheTrees[ i ] = Carry;
                        Carry = CombineTrees( T1, T2 );
                        H2->TheTrees[ i ] = NULL;
                        break;
                }
            }
            return H1;
        }
/* END */
