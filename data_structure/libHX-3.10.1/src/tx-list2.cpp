/*
 *	this program is released in the Public Domain
 */
#include <stdio.h>
#include <stdlib.h>
#include <libHX/list.h>

struct item {
	struct HXlist_head list;
};

int main(void)
{
	HXLIST_HEAD(clh);
	struct item lh;
	struct item *pos;

	HXlist_init(&lh.list);
	HXlist_add_tail(&clh, &lh.list);
	HXlist_for_each_entry(pos, &clh, list)
#ifdef OK
		printf("%p\n", pos);
#else
		;
#endif
	return EXIT_SUCCESS;
}
