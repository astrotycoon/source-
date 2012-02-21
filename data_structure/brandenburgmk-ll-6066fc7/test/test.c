#include "ll.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/timeb.h>

int dumpllitem(struct ll *list, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		list = list->next;
		if (list == NULL)
			return 1;
	}

	printf("list (%3d): 0x%016x\n"
	       "list->data: 0x%016x (char*) \"%s\"\n"
		   "list->next: 0x%016x\n\n", n, list,
		   list->data, list->data, list->next);

	return 0;
}

void dumpllitems(struct ll *list)
{
	int i, ret = 0;
	for (i = 0; ret != 1; i++) {
		ret = dumpllitem(list, i);
	}
}

int main(int argc, char **argv)
{
	struct ll *list;
	char *data = "some data";
	char *diffdata = "some other data";
	int ret, i;
	struct timeb tp;
	time_t begin, end;

	printf("Creating list.\n");
	list = ll_create();
	dumpllitem(list, 0);

	printf("Populating with data (0x%016x).\n", data);
	ret = ll_append(list, data);
	ret = ll_append(list, data);
	ret = ll_append(list, data);
	ret = ll_append(list, data);
	dumpllitems(list);

	printf("Changing last item to different data (0x%016x).\n", diffdata);
	ll_change(list, ret, diffdata);
	dumpllitems(list);

	printf("Deleting item 0.\n");
	ll_delete(list, 0);
	dumpllitems(list);

	printf("Deleting last item (%d).\n", ret-1);
	ll_delete(list, ret-1);
	dumpllitems(list);

	printf("Note that this test did not verify the data.\n"
	       "Please do that yourself.\n\n");

	printf("Testing speed.\n");
	ftime(&tp);
	begin = tp.time;
	for (i = 0; i < 1000000000; i++) {
		ll_change(list, ret-1, diffdata);
	}
	ftime(&tp);
	end = tp.time;
	printf("Took %d seconds to change 1,000,000,000 items\n", end-begin);

	return 0;
}
