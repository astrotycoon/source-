
        typedef int ElementType;

        #ifndef _PairHeap_H
        #define _PairHeap_H

        struct PairNode;
        typedef struct PairNode *PairHeap;
        typedef struct PairNode *Position;

        PairHeap PairHeap_initialize( void );
        void PairHeap_destroy( PairHeap H );
        PairHeap PairHeap_makeEmpty( PairHeap H );
        PairHeap PairHeap_insert( ElementType Item, PairHeap H, Position *Loc );
        PairHeap PairHeap_deleteMin( ElementType *MinItem, PairHeap H );
        ElementType PairHeap_findMin( PairHeap H );
        PairHeap PairHeap_decreaseKey( Position P,
                              ElementType NewVal, PairHeap H );
        int PairHeap_isEmpty( PairHeap H );
        int PairHeap_isFull( PairHeap H );
        #endif
