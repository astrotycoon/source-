        typedef int ElementType;
        #define SpaceSize 100

/* START: fig3_28.txt */
        #ifndef _Cursor_H
        #define _Cursor_H

        typedef int PtrToNode;
        typedef PtrToNode List;
        typedef PtrToNode Position;

        void ListCursor_initializeCursorSpace( void );

        List ListCursor_MakeEmpty( List L );
        int ListCursor_isEmpty( const List L );
        int ListCursor_isLast( const Position P, const List L );
        Position ListCursor_find( ElementType X, const List L );
        void ListCursor_delete( ElementType X, List L );
        Position ListCursor_findPrevious( ElementType X, const List L );
        void ListCursor_insert( ElementType X, List L, Position P );
        void ListCursor_deleteList( List L );
        Position ListCursor_header( const List L );
        Position ListCursor_first( const List L );
        Position ListCursor_advance( const Position P );
        ElementType ListCursor_retrieve( const Position P );

        #endif    /* _Cursor_H */

/* END */
