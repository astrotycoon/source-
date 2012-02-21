#ifndef _UTIL_H_
#define _UTIL_H_

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(type, member) __compiler_offsetof(type, member)
#else
#define offsetof(type, member) ((size_t) &((type *)0)->member)
#endif

#define container_of(ptr, type, member) ({ \
	    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
	    (type *)( (char *)__mptr - offsetof(type, member) );})

#endif  /* _UTIL_H_ */
