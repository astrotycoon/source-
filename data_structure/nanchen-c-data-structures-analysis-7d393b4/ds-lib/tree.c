        #include <stdlib.h>

        #include "resourcetrack.h"
        #include "tree.h"
        #include "fatal.h"

        struct TreeNode
        {
            ElementType Element;
            SearchTree  Left;
            SearchTree  Right;
        };

/* START: fig4_17.txt */
        SearchTree
        Tree_makeEmpty( SearchTree T )
        {
            if( T != NULL )
            {
                Tree_makeEmpty( T->Left );
                Tree_makeEmpty( T->Right );
                free( T );
            }
            return NULL;
        }
/* END */

/* START: fig4_18.txt */
        Position
        Tree_find( ElementType X, SearchTree T )
        {
            if( T == NULL )
                return NULL;
            if( X < T->Element )
                return Tree_find( X, T->Left );
            else
            if( X > T->Element )
                return Tree_find( X, T->Right );
            else
                return T;
        }
/* END */

/* START: fig4_19.txt */
        Position
        Tree_findMin( SearchTree T )
        {
            if( T == NULL )
                return NULL;
            else
            if( T->Left == NULL )
                return T;
            else
                return Tree_findMin( T->Left );
        }
/* END */

/* START: fig4_20.txt */
        Position
        Tree_findMax( SearchTree T )
        {
            if( T != NULL )
                while( T->Right != NULL )
                    T = T->Right;

            return T;
        }
/* END */

/* START: fig4_22.txt */
        SearchTree
        Tree_insert( ElementType X, SearchTree T )
        {
/* 1*/      if( T == NULL )
            {
                /* Create and return a one-node tree */
/* 2*/          T = malloc( sizeof( struct TreeNode ) );
                Resource_logTime(2);
/* 3*/          if( T == NULL )
/* 4*/              fatalError( "Out of space!!!" );
                else
                {
/* 5*/              T->Element = X;
/* 6*/              T->Left = T->Right = NULL;
                    Resource_logTime(2);
                }
            }
            else
/* 7*/      if( X < T->Element )
/* 8*/          T->Left = Tree_insert( X, T->Left );
            else
/* 9*/      if( X > T->Element )
/*10*/          T->Right = Tree_insert( X, T->Right );
            /* Else X is in the tree already; we'll do nothing */

/*11*/      return T;  /* Do not forget this line!! */
        }
/* END */

/* START: fig4_25.txt */
        SearchTree
        Tree_delete( ElementType X, SearchTree T )
        {
            Position TmpCell;

            if( T == NULL )
                error( "Element not found" );
            else
            if( X < T->Element )  /* Go left */
                T->Left = Tree_delete( X, T->Left );
            else
            if( X > T->Element )  /* Go right */
                T->Right = Tree_delete( X, T->Right );
            else  /* Found element to be deleted */
            if( T->Left && T->Right )  /* Two children */
            {
                /* Replace with smallest in right subtree */
                TmpCell = Tree_findMin( T->Right );
                T->Element = TmpCell->Element;
                T->Right = Tree_delete( T->Element, T->Right );
            }
            else  /* One or zero children */
            {
                TmpCell = T;
                if( T->Left == NULL ) /* Also handles 0 children */
                    T = T->Right;
                else if( T->Right == NULL )
                    T = T->Left;
                free( TmpCell );
            }

            return T;
        }
/* END */

        ElementType
        Tree_retrieve( Position P )
        {
            return P->Element;
        }
