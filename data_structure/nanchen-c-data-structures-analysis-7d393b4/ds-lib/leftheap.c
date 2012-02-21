        #include "leftheap.h"
        #include "fatal.h"
        #include <stdlib.h>


        struct TreeNode
        {
            ElementType   Element;
            PriorityQueue Left;
            PriorityQueue Right;
            int           Npl;
        };

        PriorityQueue
        LeftistHeap_initialize( void )
        {
            return NULL;
        }
        static PriorityQueue Merge1( PriorityQueue H1, PriorityQueue H2 );

/* START: fig6_26.txt */
        PriorityQueue
        LeftistHeap_merge( PriorityQueue H1, PriorityQueue H2 )
        {
/* 1*/      if( H1 == NULL )
/* 2*/          return H2;
/* 3*/      if( H2 == NULL )
/* 4*/          return H1;
/* 5*/      if( H1->Element < H2->Element )
/* 6*/          return Merge1( H1, H2 );
            else
/* 7*/          return Merge1( H2, H1 );
        }
/* END */

        void
        SwapChildren( PriorityQueue H )
        {
            PriorityQueue Tmp;

            Tmp = H->Left;
            H->Left = H->Right;
            H->Right = Tmp;
        }

/* START: fig6_27.txt */
        static PriorityQueue
        Merge1( PriorityQueue H1, PriorityQueue H2 )
        {
/* 1*/      if( H1->Left == NULL )  /* Single node */
/* 2*/          H1->Left = H2;      /* H1->Right is already NULL,
                                       H1->Npl is already 0 */
            else
            {
/* 3*/          H1->Right = LeftistHeap_merge( H1->Right, H2 );
/* 4*/          if( H1->Left->Npl < H1->Right->Npl )
/* 5*/              SwapChildren( H1 );

/* 6*/          H1->Npl = H1->Right->Npl + 1;
            }
/* 7*/      return H1;
        }
/* END */

/* START: fig6_29.txt */

        PriorityQueue
        LeftistHeap_insert1( ElementType X, PriorityQueue H )
        {
            PriorityQueue SingleNode;

/* 1*/      SingleNode = malloc( sizeof( struct TreeNode ) );
/* 2*/      if( SingleNode == NULL )
/* 3*/          FatalError( "Out of space!!!" );
            else
            {
/* 4*/          SingleNode->Element = X; SingleNode->Npl = 0;
/* 5*/          SingleNode->Left = SingleNode->Right = NULL;
/* 6*/          H = LeftistHeap_merge( SingleNode, H );
            }
/* 7*/      return H;
        }
/* END */


/* START: fig6_30.txt */
        /* LeftistHeap_deleteMin1 returns the new tree; */
        /* To get the minimum, use LeftistHeap_findMin */
        /* This is for convenience */

        PriorityQueue
        LeftistHeap_deleteMin1( PriorityQueue H )
        {
            PriorityQueue LeftHeap, RightHeap;

/* 1*/      if( LeftistHeap_isEmpty( H ) )
            {
/* 2*/          Error( "Priority queue is empty" );
/* 3*/          return H;
            }

/* 4*/      LeftHeap = H->Left;
/* 5*/      RightHeap = H->Right;
/* 6*/      free( H );
/* 7*/      return LeftistHeap_merge( LeftHeap, RightHeap );
        }
/* END */

        ElementType
        LeftistHeap_findMin( PriorityQueue H )
        {
            if( !LeftistHeap_isEmpty( H ) )
                return H->Element;
            Error( "Priority Queue is Empty" );
            return  0;
        }

        int
        LeftistHeap_isEmpty( PriorityQueue H )
        {
            return H == NULL;
        }

