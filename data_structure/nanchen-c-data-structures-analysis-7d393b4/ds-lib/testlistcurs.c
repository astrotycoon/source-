#include <stdio.h>
#include "listcursor.h"

void
PrintList( const List L )
{
    Position P = ListCursor_header( L );

    if( ListCursor_isEmpty( L ) )
        printf( "Empty list\n" );
    else
    {
        do
        {
            P = ListCursor_advance( P );
            printf( "%d ", ListCursor_retrieve( P ) );
        } while( !ListCursor_isLast( P, L ) );
        printf( "\n" );
    }
}

ListCursor_test( )
{
    List L;
    Position P;
    int i;

    ListCursor_initializeCursorSpace( );
    L = ListCursor_MakeEmpty( NULL );
    P = ListCursor_header( L );
    PrintList( L );

    for( i = 0; i < 10; i++ )
    {
        ListCursor_insert( i, L, P );
        PrintList( L );
        P = ListCursor_advance( P );
    }
    for( i = 0; i < 10; i+= 2 )
        ListCursor_delete( i, L );

    for( i = 0; i < 10; i++ )
        if( ( i % 2 == 0 ) == ( ListCursor_find( i, L ) != NULL ) )
            printf( "Find fails\n" );

    printf( "Finished deletions\n" );

    PrintList( L );

    ListCursor_deleteList( L );

    return 0;
}
