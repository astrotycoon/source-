#include <stdlib.h>
#include "fatal.h"

typedef int ElementType;

#ifndef _AATree_H
#define _AATree_H

struct AANode;
typedef struct AANode *Position;
typedef struct AANode *AATree;

AATree AATree_makeEmpty(AATree T);
Position AATree_find(ElementType X, AATree T);
Position AATree_findMin(AATree T);
Position AATree_findMax(AATree T);
AATree AATree_initialize(void);
AATree AATree_insert(ElementType X, AATree T);
AATree AATree_remove(ElementType X, AATree T);
ElementType AATree_retrieve(Position P);

extern Position NullNode;

#endif  /* _AATree_H */

/* END */
