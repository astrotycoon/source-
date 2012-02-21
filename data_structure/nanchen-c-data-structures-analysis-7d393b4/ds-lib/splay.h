        #include <stdlib.h>
        #include "fatal.h"

        typedef int ElementType;
        #define Infinity 30000
        #define NegInfinity (-30000)

/* START: fig12_5.txt */
        #ifndef _Splay_H
        #define _Splay_H

        struct SplayNode;
        typedef struct SplayNode *SplayTree;

        SplayTree SplayTree_makeEmpty( SplayTree T );
        SplayTree SplayTree_find( ElementType X, SplayTree T );
        SplayTree SplayTree_findMin( SplayTree T );
        SplayTree SplayTree_findMax( SplayTree T );
        SplayTree SplayTree_initialize( void );
        SplayTree SplayTree_insert( ElementType X, SplayTree T );
        SplayTree SplayTree_remove( ElementType X, SplayTree T );
        ElementType SplayTree_retrieve( SplayTree T );  /* Gets root item */

        #endif  /* _Splay_H */
/* END */
