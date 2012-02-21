        #include <stdlib.h>
        #include "fatal.h"

        typedef int ElementType;
        #define Infinity (10000)

        #ifndef _SkipList_H
        #define _SkipList_H

        struct SkipNode;
        typedef struct SkipNode *Position;
        typedef struct SkipNode *SkipList;

        SkipList DSList_makeEmpty( SkipList L );
        Position DSList_find( ElementType X, SkipList L );
        Position DSList_findMin( SkipList L );
        Position DSList_findMax( SkipList L );
        SkipList DSList_initialize( void );
        SkipList DSList_insert( ElementType X, SkipList L );
        SkipList DSList_remove( ElementType X, SkipList L );
        ElementType DSList_retrieve( Position P );

        #endif  /* _SkipList_H */

/* END */
