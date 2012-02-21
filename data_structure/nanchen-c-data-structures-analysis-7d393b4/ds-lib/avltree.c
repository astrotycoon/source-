#include "avltree.h"
#include "fatal.h"
#include "resourcetrack.h"

#include <stdlib.h>

struct AvlNode {
	AVLTreeElement Element;
	AvlTree Left;
	AvlTree Right;
	int Height;
};

const int SPACE_TREE = sizeof(AvlTree);
const int SPACE_NODE = sizeof(struct AvlNode);
const int SPACE_TREE_ELE = sizeof(AvlTree) + sizeof(AVLTreeElement);
const int SPACE_POS = sizeof(Position);

AvlTree AVLTree_makeEmpty(AvlTree T) {
	Resource_logSpace(SPACE_TREE);
	if (T != NULL) //1
	{
		AVLTree_makeEmpty(T->Left); //1
		AVLTree_makeEmpty(T->Right); //1
		free(T); //1
		Resource_logTime(3);
		Resource_logSpace(-SPACE_NODE);
	}
	Resource_logTime(3);
	Resource_logSpace(-SPACE_TREE);
	return NULL;
}

Position AVLTree_find(AVLTreeElement X, AvlTree T) {
	Resource_logSpace(SPACE_TREE_ELE);
	Resource_logSpace(-SPACE_TREE_ELE);

	if (T == NULL) { //1
		Resource_logTime(3);
		return NULL;
	}
	if (X < T->Element) { //2
		Resource_logTime(6);
		return AVLTree_find(X, T->Left); //1
	} else if (X > T->Element){ //2
		Resource_logTime(8);
		return AVLTree_find(X, T->Right); //1
	}
	else{ //1
		Resource_logTime(8);
		return T;
	}
}

Position AVLTree_findMin(AvlTree T) {
	Resource_logSpace(SPACE_TREE);
	Resource_logSpace(-SPACE_TREE);
	if (T == NULL){ //1
		Resource_logTime(3);
		return NULL;
	}
	else if (T->Left == NULL){ //1
		Resource_logTime(4);
		return T;
	}
	else{
		Resource_logTime(6);
		return AVLTree_findMin(T->Left);
	}
}

Position AVLTree_findMax(AvlTree T) {
	Resource_logSpace(SPACE_TREE);
	Resource_logSpace(-SPACE_TREE);
	if (T != NULL) //1
		while (T->Right != NULL){
			Resource_logTime(2);
			T = T->Right; //2
		}
	Resource_logTime(3);
	return T;
}

/* START: fig4_36.txt */
static int Height(Position P) {
	Resource_logSpace(SPACE_POS);
	Resource_logSpace(-SPACE_POS);
	Resource_logTime(4);
	if (P == NULL)
		return -1;
	else
		return P->Height;
}
/* END */

static int Max(int Lhs, int Rhs) {
	const int SPACE = sizeof(int) *2;
	Resource_logSpace(SPACE);
	Resource_logSpace(-SPACE);
	Resource_logTime(3);
	return Lhs > Rhs ? Lhs : Rhs;
}

/* START: fig4_39.txt */
/* This function can be called only if K2 has a left child */
/* Perform a rotate between a node (K2) and its left child */
/* Update heights, then return new root */

static Position SingleRotateWithLeft(Position K2) {
	Resource_logSpace(SPACE_POS);
	Resource_logSpace(-SPACE_POS);
	Resource_logTime(19);

	Position K1; //1

	K1 = K2->Left; //2
	K2->Left = K1->Right; //3
	K1->Right = K2; //2

	K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1; //4
	K1->Height = Max(Height(K1->Left), K2->Height) + 1; //5

	return K1; /* New root */
}
/* END */

/* This function can be called only if K1 has a right child */
/* Perform a rotate between a node (K1) and its right child */
/* Update heights, then return new root */

static Position SingleRotateWithRight(Position K1) {
	Resource_logSpace(SPACE_POS);
	Resource_logSpace(-SPACE_POS);
	Resource_logTime(19);

	Position K2;

	K2 = K1->Right;
	K1->Right = K2->Left;
	K2->Left = K1;

	K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
	K2->Height = Max(Height(K2->Right), K1->Height) + 1;

	return K2; /* New root */
}

/* START: fig4_41.txt */
/* This function can be called only if K3 has a left */
/* child and K3's left child has a right child */
/* Do the left-right double rotation */
/* Update heights, then return new root */

static Position DoubleRotateWithLeft(Position K3) {
	Resource_logSpace(SPACE_POS);
	Resource_logSpace(-SPACE_POS);
	Resource_logTime(5);

	/* Rotate between K1 and K2 */
	K3->Left = SingleRotateWithRight(K3->Left); //3

	/* Rotate between K3 and K2 */
	return SingleRotateWithLeft(K3);
}
/* END */

/* This function can be called only if K1 has a right */
/* child and K1's right child has a left child */
/* Do the right-left double rotation */
/* Update heights, then return new root */

static Position DoubleRotateWithRight(Position K1) {
	Resource_logSpace(SPACE_POS);
	Resource_logSpace(-SPACE_POS);
	Resource_logTime(5);

	/* Rotate between K3 and K2 */
	K1->Right = SingleRotateWithLeft(K1->Right);

	/* Rotate between K1 and K2 */
	return SingleRotateWithRight(K1);
}

/* START: fig4_37.txt */
AvlTree AVLTree_insert(AVLTreeElement X, AvlTree T) {
	Resource_logSpace(SPACE_TREE_ELE);
	if (T == NULL) //1
	{
		/* Create and return a one-node tree */
		T = malloc(sizeof(struct AvlNode)); //3
		Resource_logSpace(sizeof(struct AvlNode));
		if (T == NULL) //1
			fatalError( "Out of space!!!" );
		else {
			T->Element = X; //2
			T->Height = 0; //2
			T->Left = T->Right = NULL; //4
			Resource_logTime(8);
		}
		Resource_logTime(1);
	} else if (X < T->Element) //2
	{
		T->Left = AVLTree_insert(X, T->Left); //3
		if (Height(T->Left) - Height(T->Right) == 2) //4
			if (X < T->Left->Element)
				T = SingleRotateWithLeft(T); //1
			else
				T = DoubleRotateWithLeft(T); //1
		Resource_logTime(9);
	} else if (X > T->Element) //2
	{
		T->Right = AVLTree_insert(X, T->Right);
		if (Height(T->Right) - Height(T->Left) == 2)
			if (X > T->Right->Element)
				T = SingleRotateWithRight(T);
			else
				T = DoubleRotateWithRight(T);
		Resource_logTime(7);
	}
	/* Else X is in the tree already; we'll do nothing */

	T->Height = Max(Height(T->Left), Height(T->Right)) + 1; //5
	Resource_logTime(12);
	Resource_logSpace(-SPACE_TREE_ELE);
	return T;
}
/* END */

AvlTree AVLTree_delete(AVLTreeElement X, AvlTree T) {
	printf("Sorry; Delete is unimplemented; %d remains\n", X);
	return T;
}

AVLTreeElement AVLTree_retrieve(Position P) {
	Resource_logSpace(SPACE_POS);
	Resource_logSpace(-SPACE_POS);
	Resource_logTime(3);
	return P->Element;
}
