#include <stdio.h>

#include "tree.h"
#include "resourcetrack.h"

Tree_test() {
    const int NUM_OF_NODES = 50;
	SearchTree T;
	Position P;
	int i;
	int j = 0;

	T = Tree_makeEmpty(NULL);
	for (i = 0; i < NUM_OF_NODES; i++, j = (j + 7) % NUM_OF_NODES){
		T = Tree_insert(j, T);
	}
	for (i = 0; i < NUM_OF_NODES; i++)
		if ((P = Tree_find(i, T)) == NULL || Tree_retrieve(P) != i)
			printf("Error at %d\n", i);

	for (i = 0; i < NUM_OF_NODES; i += 2)
		T = Tree_delete(i, T);

	for (i = 1; i < NUM_OF_NODES; i += 2)
		if ((P = Tree_find(i, T)) == NULL || Tree_retrieve(P) != i)
			printf("Error at %d\n", i);
	for (i = 0; i < NUM_OF_NODES; i += 2)
		if ((P = Tree_find(i, T)) != NULL)
			printf("Error at %d\n", i);

	printf("Min is %d, Max is %d\n", Tree_retrieve(Tree_findMin(T)),
			Tree_retrieve(Tree_findMax(T)));

	return 0;
}
