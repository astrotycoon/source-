
        typedef int ElementType;

/* START: fig6_4.txt */
        #ifndef _BinHeap_H
        #define _BinHeap_H

        struct HeapStruct;
        typedef struct HeapStruct *PriorityQueue;

        PriorityQueue BinaryHeap_initialize( int MaxElements );
        void BinaryHeap_destroy( PriorityQueue H );
        void BinaryHeap_makeEmpty( PriorityQueue H );
        void BinaryHeap_insert( ElementType X, PriorityQueue H );
        ElementType BinaryHeap_deleteMin( PriorityQueue H );
        ElementType BinaryHeap_findMin( PriorityQueue H );
        int BinaryHeap_isEmpty( PriorityQueue H );
        int BinaryHeap_isFull( PriorityQueue H );

        #endif

/* END */
