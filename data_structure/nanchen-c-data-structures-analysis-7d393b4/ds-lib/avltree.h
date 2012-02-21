        typedef int AVLTreeElement;

/* START: fig4_35.txt */
        #ifndef _AvlTree_H
        #define _AvlTree_H

        struct AvlNode;
        typedef struct AvlNode *Position;
        typedef struct AvlNode *AvlTree;

        AvlTree AVLTree_makeEmpty( AvlTree T );
        Position AVLTree_find( AVLTreeElement X, AvlTree T );
        Position AVLTree_findMin( AvlTree T );
        Position AVLTree_findMax( AvlTree T );
        AvlTree AVLTree_insert( AVLTreeElement X, AvlTree T );
        AvlTree AVLTree_delete( AVLTreeElement X, AvlTree T );
        AVLTreeElement AVLTree_retrieve( Position P );

        #endif  /* _AvlTree_H */
/* END */
