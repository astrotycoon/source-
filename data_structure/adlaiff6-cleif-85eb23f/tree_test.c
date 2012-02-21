#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <tree.h>
#include <error.h>
#include <test.h>

int cmp(const void *p1, const void *p2, __attribute__((unused)) void *data)
{
	if (*((int *)p1) < *((int *)p2)) {
		return -1;
	} else if (*((int *)p1) > *((int *)p2)) {
		return 1;
	} else {
		return 0;
	}
}

void act(const void *val, void *counters)
{
	((int *)counters)[0]++;
	((int *)counters)[1] += *((int *)val);
}

int main(void)
{
	int i, counters[2];
	struct tree *t;

	t = tnew(cmp, NULL);

	for (i=0; i<100; ++i) {
		int *val;
		val = (int *) malloc(sizeof(int));
		assert(val);
		*val = i;
		tinsert(t, val);
	}

	counters[0] = counters[1] = 0;
	preorder(t, act, counters);
	if (counters[0] != 100) {
		fail("there should be 100 items in the tree");
	}
	if (counters[1] != 4950) {
		fail("tree should sum to 4950");
	}

	printf("success!\n");

	return 0;
}
