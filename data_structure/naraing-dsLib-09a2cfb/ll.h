/*  Copyright 2010 Gaurav Mathur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DS_SLL_H
#define DS_SLL_H

#include <ds_types.h>

#define DEFAULT_SKL_MAX_LEVEL   (16)
#define DEFAULT_SKL_P           (0.5)
#define DEFAULT_SKL_START_LEVEL (1)

#define ARG_TO_DATA(ll, data)                                           \
   do {                                                                 \
      va_list ap;                                                       \
      va_start (ap, ll);                                                \
      switch (ll->data_type)                                            \
      {                                                                 \
         case IDATA:                                                    \
         case CDATA:                                                    \
            data.idata = va_arg (ap, int);                              \
            break;                                                      \
         case UIDATA:                                                   \
         case UCDATA:                                                   \
            data.uidata = va_arg (ap, unsigned int);                    \
            break;                                                      \
         case VPDATA:                                                   \
            data.vpdata = va_arg (ap, void*);                           \
            break;                                                      \
         case FPDATA:                                                   \
            data.fpdata = va_arg (ap, FUNCPTR2_T);                      \
            break;                                                      \
         case LDATA:                                                    \
         case LLDATA:                                                   \
         case ULDATA:                                                   \
         case ULLDATA:                                                  \
            /* todo */                                                  \
            break;                                                      \
      }                                                                 \
      va_end (ap);                                                      \
   }while (0)                                                                     
   
/* single linked list node */
typedef struct _sllnode
   {
      struct _sllnode* next;
#define FIDATA   item.idata
#define FUIDATA  item.uidata
#define FCDATA   item.cdata    
#define FUCDATA  item.ucdata    
#define FVPDATA  item.vpdata
#define FFPDATA  item.fpdata 
    NODE_DATA_TYPE_E type;
    NODE_DATA_T item;       
    }SLL_NODE;

typedef int (*SKL_ITER_FUNC)(SLL_NODE*);
typedef int (*SLL_ITER_FUNC)(SLL_NODE*);

/* Single linked list meta node */
typedef struct _sllnodehead {
   struct _sllnode* lhead;
   struct _sllnode* ltail;
   int total_elements;
   NODE_DATA_TYPE_E data_type;
   SLL_ITER_FUNC iterator;
   FUNCPTR2_T preiterator;
   FUNCPTR2_T postiterator;
   unsigned char max_level;
} SLL_T;

/* Skip list node type */
typedef struct _sklnode {
   /* array of level N next nodes */
   struct _sklnode** nexts;
#define FIDATA   item.idata
#define FUIDATA  item.uidata
#define FCDATA   item.cdata    
#define FUCDATA  item.ucdata    
#define FVPDATA  item.vpdata
#define FFPDATA  item.fpdata 
   NODE_DATA_T item;
   unsigned char level;
}SKL_NODE;

/* Skip list meta node */
typedef struct _skl {
   /* This is a real node, not just a pointer to the first node of the
    * list. It's item field is not filled or used though.
    */
   SKL_NODE* head;
   /* Total number of elements in this list */
   int total_elements;
   /* Type of data the list will contain */
   NODE_DATA_TYPE_E data_type;
   /* This function will be called for every node that's looked up in a SKL interator */   
   SLL_ITER_FUNC iterator;
   /* This function will be at the start of an SKL iterator */   
   FUNCPTR2_T preiterator;
   /* This function will be at the end of an SKL iterator */   
   FUNCPTR2_T postiterator;
   /* The maximum level supported in this skip list */
   unsigned char max_level;
   /* Current maximum level used by a node in the list */
   unsigned char cur_level;
} SKL_T; 

typedef struct _dllnode
    {
    struct _dllnode* next;
    struct _dllnode* prev;
    int item;
    }DLL_NODE;

typedef enum {
   ERR_LL_ERROR_HIGH = -512, /* fencepost */
   ERR_LL_ELE_NOT_FOUND, /* element not found in the list */
   ERR_LL_MALLOC_FAIL,
   ERR_LL_ERR = -1, /* generic operational error */
   ERR_LL_OK = 0 /* routine returned without any errors */
} ERR_LL_E;


/* Public API */
/* insert an element into the SLL */
ERR_LL_E        sll_insert (SLL_T* head, ...);
/* insert N element into the SLL, all of the same types */
ERR_LL_E        sll_ninsert (SLL_T* head, unsigned int num, ...);
/* delete an element from the SLL */
ERR_LL_E        sll_delete (SLL_T* sll, ...);
/* create a new SLL */
SLL_T*          sll_new (NODE_DATA_TYPE_E data_type, SLL_ITER_FUNC iterator,
                         FUNCPTR2_T pre, FUNCPTR2_T post);
/* iterate over all the elements of the SLL */
void            sll_iter (SLL_T* head);
/* find the SLL node object next to the given one */
SLL_NODE*       sll_next_get (SLL_T* sll, SLL_NODE* to);
/* find the SLL node object previous to the given one */
SLL_NODE*       sll_prev_get (SLL_T* sll, SLL_NODE* to);
/* get the first node in the SLL */
SLL_NODE*       sll_first_get (SLL_T* sll);
/* get the last node in the SLL */
SLL_NODE*       sll_last_get (SLL_T* sll);
/* remove the first element in the SLL */
SLL_NODE*       sll_remove_first (SLL_T* sll);
/* expand a linked list */
ERR_LL_E        sll_concat (SLL_T* first, SLL_T* second);
/* find node with the specified element value */
SLL_NODE* sll_find (SLL_T* sll, ...);
/* create a new skip list */
SKL_T* skl_new (NODE_DATA_TYPE_E data_type, unsigned char max_level,
                SKL_ITER_FUNC iterator, FUNCPTR2_T pre, FUNCPTR2_T post);

#endif /* !defined DS_SLL_H */
