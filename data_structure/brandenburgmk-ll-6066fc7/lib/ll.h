#ifndef LL_H
#define LL_H

struct ll {
	void *data;
	struct ll *next;
};

#define SIZEOF_LL (sizeof(struct ll))

struct ll *ll_create(void);
int ll_append(struct ll *p, void *data);
int ll_change(struct ll *p, int n, void *data);
void *ll_get(struct ll *list, int n);
int ll_lookup(struct ll *list, void *data);
int ll_delete(struct ll *p, int n);

#endif
