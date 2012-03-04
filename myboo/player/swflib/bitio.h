#ifndef _BITIO_H_
#define _BITIO_H_

#include "types.h"

#define READER_TYPE_FILE	1
#define READER_TYPE_FILE2	6

typedef struct _reader
{
  int (*read)(struct _reader*, void* data, int len);
  int (*seek)(struct _reader*, int pos);
  void (*dealloc)(struct _reader*);

  void* internal;
  int type;
  unsigned char mybyte;
  unsigned char bitpos;
  int pos;
} reader_t;


void reader_init_filereader(reader_t*, int);

unsigned int reader_readbits(reader_t* r, int num);

U32 reader_readU32(reader_t* r);

#endif /* _BITIO_H_ */
