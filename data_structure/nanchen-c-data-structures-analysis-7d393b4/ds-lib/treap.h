        #include <stdlib.h>
        #include "fatal.h"

        typedef int ElementType;
        #define Infinity 32767

        #ifndef _Treap_H
        #define _Treap_H

        struct TreapNode;
        typedef struct TreapNode *Position;
        typedef struct TreapNode *Treap;

        Treap Treap_makeEmpty( Treap T );
        Position Treap_find( ElementType X, Treap T );
        Position Treap_findMin( Treap T );
        Position Treap_findMax( Treap T );
        Treap Treap_initialize( void );
        Treap Treap_insert( ElementType X, Treap T );
        Treap Treap_remove( ElementType X, Treap T );
        ElementType Treap_retrieve( Position P );


        extern Position NullNode;

        #endif  /* _Treap_H */

/* END */
