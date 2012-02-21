#include <stdio.h>
#include <stdlib.h>

#define error( Str )        fatalError( Str )
#define fatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )
