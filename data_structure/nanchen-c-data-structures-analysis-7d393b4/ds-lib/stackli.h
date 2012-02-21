        typedef int ElementType;
/* START: fig3_39.txt */
        #ifndef _Stack_h
        #define _Stack_h

        struct Node;
        typedef struct Node *PtrToNode;
        typedef PtrToNode Stack;

        int Stack_isEmpty( Stack S );
        Stack Stack_createStack( void );
        void Stack_disposeStack( Stack S );
        void Stack_makeEmpty( Stack S );
        void Stack_push( ElementType X, Stack S );
        ElementType Stack_top( Stack S );
        void Stack_pop( Stack S );

        #endif  /* _Stack_h */

/* END */
