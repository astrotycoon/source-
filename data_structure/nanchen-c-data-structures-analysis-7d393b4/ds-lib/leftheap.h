
        typedef int ElementType;

/* START: fig6_25.txt */
        #ifndef _LeftHeap_H
        #define _LeftHeap_H

        struct TreeNode;
        typedef struct TreeNode *PriorityQueue;

        /* Minimal set of priority queue operations */
        /* Note that nodes will be shared among several */
        /* leftist heaps after a merge; the user must */
        /* make sure to not use the old leftist heaps */

        PriorityQueue LeftistHeap_initialize( void );
        ElementType LeftistHeap_findMin( PriorityQueue H );
        int LeftistHeap_isEmpty( PriorityQueue H );
        PriorityQueue LeftistHeap_merge( PriorityQueue H1, PriorityQueue H2 );

        #define Insert( X, H ) ( H = LeftistHeap_insert1( ( X ), H ) )
        /* DeleteMin macro is left as an exercise */

        PriorityQueue LeftistHeap_insert1( ElementType X, PriorityQueue H );
        PriorityQueue LeftistHeap_deleteMin1( PriorityQueue H );

        #endif

/* END */
