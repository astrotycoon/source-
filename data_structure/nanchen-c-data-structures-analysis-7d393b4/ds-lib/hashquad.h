/* Interface for quadratic probing hash table */
typedef int ElementType;

/* START: fig5_14.txt */
        #ifndef _HashQuad_H
        #define _HashQuad_H

        typedef unsigned int Index;
        typedef Index Position;

        struct HashTbl;
        typedef struct HashTbl *HashTable;

        HashTable Hash_initializeTable( int TableSize );
        void Hash_destroyTable( HashTable H );
        Position Hash_find( ElementType Key, HashTable H );
        void Hash_insert( ElementType Key, HashTable H );
        ElementType Hash_retrieve( Position P, HashTable H );
        HashTable Hash_rehash( HashTable H );
        /* Routines such as Hash_delete are Hash_makeEmpty are omitted */

        #endif  /* _HashQuad_H */

/* END */
