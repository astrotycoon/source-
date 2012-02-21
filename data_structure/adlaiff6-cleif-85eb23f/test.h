#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <stdlib.h>

#include "error.h"

#define fail(msg, args...) do { \
	error("TEST FAILURE: %s\n", (msg), ##args); \
	exit(2); \
} while(0)

#define _assert(cond) do { \
	if (!cond) { fail( #cond ); } \
} while (0)

#endif  /* _TEST_H_ */
