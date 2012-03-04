/*
*/
#include <stdlib.h>


void* rfx_calloc(unsigned int size)
{
void* ptr = NULL;

	if(0 == size) return 0;

	ptr = malloc(size);

	if(NULL == ptr)
	{
		return NULL;
	}

	memset(ptr, 0, size);

	return ptr;
}

void rfx_free(void* data)
{
	if(NULL == data) return;
	free(data);
}
