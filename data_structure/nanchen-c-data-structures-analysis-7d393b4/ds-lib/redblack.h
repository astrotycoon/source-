        #include <stdlib.h>
        #include "fatal.h"

        typedef int ElementType;
        #define NegInfinity (-10000)

        #ifndef _RedBlack_H
        #define _RedBlack_H

        struct RedBlackNode;
        typedef struct RedBlackNode *Position;
        typedef struct RedBlackNode *RedBlackTree;

        RedBlackTree RedBlackTree_makeEmpty( RedBlackTree T );
        Position RedBlackTree_find( ElementType X, RedBlackTree T );
        Position RedBlackTree_findMin( RedBlackTree T );
        Position RedBlackTree_findMax( RedBlackTree T );
        RedBlackTree RedBlackTree_initialize( void );
        RedBlackTree RedBlackTree_insert( ElementType X, RedBlackTree T );
        RedBlackTree RedBlackTree_remove( ElementType X, RedBlackTree T );
        ElementType RedBlackTree_retrieve( Position P );
        void RedBlackTree_printTree( RedBlackTree T );

        #endif  /* _RedBlack_H */

/* END */
