/* This file is for testing the cumulative include */
#ifndef __cplusplus
#	include <stdlib.h>
#else
#	include <cstdlib>
#endif
#include <libHX.h>

#define ZZ 64

int main(void)
{
	unsigned long long bmllong[HXbitmap_size(unsigned long long, 256)];
	unsigned long bmlong[HXbitmap_size(unsigned long, 256)];
	unsigned int bmint[HXbitmap_size(unsigned int, 256)];
	unsigned short bmshort[HXbitmap_size(unsigned short, 256)];
	unsigned char bmchar[HXbitmap_size(unsigned char, 256)];

	if (HX_init() <= 0)
		abort();
	printf("sizeof bmllong:\t%zu, array_size: %zu\n",
	       sizeof(bmllong), ARRAY_SIZE(bmllong));
	printf("sizeof bmlong:\t%zu, array_size: %zu\n",
	       sizeof(bmlong), ARRAY_SIZE(bmlong));
	printf("sizeof bmint:\t%zu, array_size: %zu\n",
	       sizeof(bmint), ARRAY_SIZE(bmint));
	printf("sizeof bmshort:\t%zu, array_size: %zu\n",
	       sizeof(bmshort), ARRAY_SIZE(bmshort));
	printf("sizeof bmchar:\t%zu, array_size: %zu\n",
	       sizeof(bmchar), ARRAY_SIZE(bmchar));
	HXbitmap_set(bmllong, 255);
	HXbitmap_set(bmlong, 255);
	HXbitmap_set(bmint, 255);
	HXbitmap_set(bmshort, 255);
	HXbitmap_set(bmchar, 255);
	printf(HX_STRINGIFY(1234+2 +2) "," HX_STRINGIFY(ZZ) "\n");
	HX_exit();
	return EXIT_SUCCESS;
}
