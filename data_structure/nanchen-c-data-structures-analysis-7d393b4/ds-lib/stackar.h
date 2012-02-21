        typedef int ElementType;
/* START: fig3_45.txt */
        #ifndef _Stack_h
        #define _Stack_h

        struct StackRecord;
        typedef struct StackRecord *Stack;

        int Stack_isEmpty( Stack S );
        int Stack_isFull( Stack S );
        Stack Stack_createStack( int MaxElements );
        void Stack_disposeStack( Stack S );
        void Stack_makeEmpty( Stack S );
        void Stack_push( ElementType X, Stack S );
        ElementType Stack_top( Stack S );
        void Stack_pop( Stack S );
        ElementType Stack_topAndPop( Stack S );

        #endif  /* _Stack_h */

/* END */
