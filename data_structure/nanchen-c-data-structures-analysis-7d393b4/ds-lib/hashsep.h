/* Interface for separate chaining hash table */
typedef int ElementType;

/* START: fig5_2.txt */
        typedef unsigned int Index;
/* END */

/* START: fig5_7.txt */
        #ifndef _HashSep_H
        #define _HashSep_H

        struct ListNode;
        typedef struct ListNode *Position;
        struct HashTbl;
        typedef struct HashTbl *HashTable;

        HashTable Hash_initializeTable( int TableSize );
        void Hash_destroyTable( HashTable H );
        Position Hash_find( ElementType Key, HashTable H );
        void Hash_insert( ElementType Key, HashTable H );
        ElementType Hash_retrieve( Position P );
        /* Routines such as Hash_delete are Hash_makeEmpty are omitted */

        #endif  /* _HashSep_H */
/* END */
