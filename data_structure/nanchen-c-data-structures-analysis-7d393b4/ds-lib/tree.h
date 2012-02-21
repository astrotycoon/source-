typedef int ElementType;

/* START: fig4_16.txt */
#ifndef _Tree_H
#define _Tree_H

struct TreeNode;
typedef struct TreeNode *Position;
typedef struct TreeNode *SearchTree;

SearchTree Tree_makeEmpty(SearchTree T);
Position Tree_find(ElementType X, SearchTree T);
Position Tree_findMin(SearchTree T);
Position Tree_findMax(SearchTree T);
SearchTree Tree_insert(ElementType X, SearchTree T);
SearchTree Tree_delete(ElementType X, SearchTree T);
ElementType Tree_retrieve(Position P);

#endif  /* _Tree_H */

/* END */
