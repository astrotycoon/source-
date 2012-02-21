
        typedef long ElementType;
        #define Infinity (30000L)

        #ifndef _BinHeap_H
        #define _BinHeap_H

        #define MaxTrees (14)   /* Stores 2^14 -1 items */
        #define Capacity (16383)

        struct BinNode;
        typedef struct BinNode *BinTree;
        struct Collection;
        typedef struct Collection *BinQueue;

        BinQueue BinomialQueue_initialize( void );
        void BinomialQueue_destroy( BinQueue H );
        BinQueue BinomialQueue_makeEmpty( BinQueue H );
        BinQueue BinomialQueue_insert( ElementType Item, BinQueue H );
        ElementType BinomialQueue_deleteMin( BinQueue H );
        BinQueue BinomialQueue_merge( BinQueue H1, BinQueue H2 );
        ElementType BinomialQueue_findMin( BinQueue H );
        int BinomialQueue_isEmpty( BinQueue H );
        int BinomialQueue_isFull( BinQueue H );
        #endif
/* END */
