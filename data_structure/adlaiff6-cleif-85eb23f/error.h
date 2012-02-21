#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdio.h>
#include <stdlib.h>

#define error(fmt, args...) fprintf(stderr, __FILE__ ":%d: " fmt, \
		__LINE__, ##args)
#define abort(fmt, args...) do { \
	error(fmt "\n", ##args); \
	perror(""); \
	exit(1); \
} while (0)

#endif  /* _ERROR_H_ */
