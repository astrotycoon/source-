        #include "queue.h"
        #include "fatal.h"
        #include <stdlib.h>

        #define MinQueueSize ( 5 )

        struct QueueRecord
        {
            int Capacity;
            int Front;
            int Rear;
            int Size;
            ElementType *Array;
        };

/* START: fig3_58.txt */
        int
        Queue_isEmpty( Queue Q )
        {
            return Q->Size == 0;
        }
/* END */

        int
        Queue_isFull( Queue Q )
        {
            return Q->Size == Q->Capacity;
        }

        Queue
        Queue_createQueue( int MaxElements )
        {
            Queue Q;

/* 1*/      if( MaxElements < MinQueueSize )
/* 2*/          error( "Queue size is too small" );

/* 3*/      Q = malloc( sizeof( struct QueueRecord ) );
/* 4*/      if( Q == NULL )
/* 5*/          fatalError( "Out of space!!!" );

/* 6*/      Q->Array = malloc( sizeof( ElementType ) * MaxElements );
/* 7*/      if( Q->Array == NULL )
/* 8*/          fatalError( "Out of space!!!" );
/* 9*/      Q->Capacity = MaxElements;
/*10*/      Queue_makeEmpty( Q );

/*11*/      return Q;
        }

/* START: fig3_59.txt */
        void
        Queue_makeEmpty( Queue Q )
        {
            Q->Size = 0;
            Q->Front = 1;
            Q->Rear = 0;
        }
/* END */

        void
        Queue_disposeQueue( Queue Q )
        {
            if( Q != NULL )
            {
                free( Q->Array );
                free( Q );
            }
        }

/* START: fig3_60.txt */

        static int
        Succ( int Value, Queue Q )
        {
            if( ++Value == Q->Capacity )
                Value = 0;
            return Value;
        }

        void
        Queue_enqueue( ElementType X, Queue Q )
        {
            if( Queue_isFull( Q ) )
                error( "Full queue" );
            else
            {
                Q->Size++;
                Q->Rear = Succ( Q->Rear, Q );
                Q->Array[ Q->Rear ] = X;
            }
        }
/* END */



        ElementType
        Queue_front( Queue Q )
        {
            if( !Queue_isEmpty( Q ) )
                return Q->Array[ Q->Front ];
            error( "Empty queue" );
            return 0;  /* Return value used to avoid warning */
        }

        void
        Queue_dequeue( Queue Q )
        {
            if( Queue_isEmpty( Q ) )
                error( "Empty queue" );
            else
            {
                Q->Size--;
                Q->Front = Succ( Q->Front, Q );
            }
        }

        ElementType
        Queue_frontAndDequeue( Queue Q )
        {
            ElementType X = 0;

            if( Queue_isEmpty( Q ) )
                error( "Empty queue" );
            else
            {
                Q->Size--;
                X = Q->Array[ Q->Front ];
                Q->Front = Succ( Q->Front, Q );
            }
            return X;
        }
