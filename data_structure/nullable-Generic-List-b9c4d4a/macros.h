#ifndef __MACROS_H__
#define __MACROS_H__

#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#define log(level, format, ...)					\
  do{								\
    fprintf(stderr, "%s  %s:%d: ", level, __FILE__, __LINE__);	\
    fprintf(stderr, format, ## __VA_ARGS__);			\
    fprintf(stderr, "\n");					\
  }while(0)
#else
#define log(level, str, ...)
#endif

#define I "INFO"
#define W "WARNNING"
#define E "ERROR"
#define F "FATAL ERROR"

#define alloc(type, how_many)				\
  (type *) __alloc(malloc(how_many * sizeof(type)));	

static inline void* __alloc(void* x){
  if(x)
    return x;
  log(F,"malloc failed.");				
  exit(1);
  return 0;
}

#endif
    
