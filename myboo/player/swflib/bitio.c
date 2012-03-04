/*
*/
#include <io.h>
#include <stdio.h>
#include <memory.h>

#include "swflib.h"
#include "bitio.h"

/* ------------------- file reader ----------------------------*/
static int reader_fileread(reader_t* r, void* data, int len)
{
int ret;
	ret = _read((int)r->internal, data, len);
	if( ret >= 0 )
		r->pos += ret;
	return ret;
}

static long reader_fileread_seek(reader_t* r, int pos)
{
	return _lseek((int)r->internal, pos, SEEK_SET);
}

static void reader_fileread_dealloc(reader_t* r)
{
	if( r->type == READER_TYPE_FILE2)
	{
		_close((int)r->internal);
	}
	memset(r, 0, sizeof(reader_t));
}


void reader_init_filereader(reader_t* r, int handle)
{
	r->read = reader_fileread;
	r->seek = reader_fileread_seek;
	r->dealloc = reader_fileread_dealloc;
	r->internal = (void*)handle;
	r->type = READER_TYPE_FILE;
	r->mybyte = 0;
	r->bitpos = 8;
	r->pos = 0;
}


unsigned int reader_readbit(reader_t* r)
{
	if(8 == r->bitpos)
	{
		r->bitpos = 0;
		r->read(r, &r->mybyte, 1);
	}
	return (r->mybyte >> (7 - r->bitpos++)) & 1;
}

unsigned int reader_readbits(reader_t* r, int num)
{
int t;
int val = 0;

	for(t=0; t<num; t++)
	{
		val <<= 1;
		val |= reader_readbit(r);
	}
	return val;
}

U32 reader_readU32(reader_t* r)
{
U8 b1=0, b2=0, b3=0, b4=0;
	
	if(r->read(r, &b1, 1) < 1);
	if(r->read(r, &b2, 1) < 1);
	if(r->read(r, &b3, 1) < 1);
	if(r->read(r, &b4, 1) < 1);

	return (b1 | b2<<8 | b3<<16 | b4<<24);
}
