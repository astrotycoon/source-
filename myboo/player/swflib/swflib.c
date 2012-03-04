/**********************************************************************************
 *
 **********************************************************************************/

#include <stdlib.h>
#include <memory.h>

#include "swflib.h"
#include "bitio.h"
#include "types.h"

#include "bitio.h"
#include "mem.h"


TAG* swf_ReadTag(reader_t* reader, TAG* prev)
{
TAG* t = NULL;
U16 raw;
U32 len;
int id;

	if(reader->read(reader, &raw, 2) != 2) return NULL;
	raw = LE_16_TO_NATIVE(raw);

	len = raw & 0x3F;  /*U16, 10 bits ~ tag type, 6 bits ~ tag len */
	id = raw >> 6;

	if(0x3F == len)  /* it is a long tag header behind */
	{
		len = reader_readU32(reader);
	}

	if(ST_DEFINESPRITE == id) len = 2 * sizeof(U16);

	t = (TAG*)rfx_calloc(sizeof(TAG));
	if(NULL == t) return NULL;
	t->len = len;
	t->id = id;

	if(t->len)
	{
		t->data = (U8*)rfx_calloc(t->len);
		if(NULL == t->data)
		{
			free(t);
			return NULL;
		}
		t->memsize = t->len;
		if(reader->read(reader, t->data, t->len) != t->len)
		{
			free(t->data);
			t->data = 0;
			free(t);
			return NULL;
		}
	}
	if(prev)
	{
		t->prev = prev;
		prev->next = t;
	}

	return t;
}


U32 reader_GetBits(reader_t* reader, int nbits)
{
	return reader_readbits(reader, nbits);
}

S32 reader_GetSBits(reader_t* reader, int nbits)
{
U32 res;

	res = reader_readbits(reader, nbits);
	if(res & (1<<(nbits-1))) res |= (0xFFFFFFFF<<nbits);

	return (S32)res;
}

int reader_GetRect(reader_t* reader, SRECT* r)
{
int nbits;
SRECT dummy;

	if(NULL == r) r = &dummy;
	nbits = (int) reader_GetBits(reader, 5);
	r->xmin = reader_GetSBits(reader, nbits);
	r->xmax = reader_GetSBits(reader, nbits);
	r->ymin = reader_GetSBits(reader, nbits);
	r->ymax = reader_GetSBits(reader, nbits);

	return 0;
}

U32 swf_GetU32(TAG* t)
{
U32 res;

	swf_ResetReadBits(t);
	res = (t->data[t->pos])       | (t->data[t->pos+1]<<8) |
	      (t->data[t->pos+2]<<16) | (t->data[t->pos+3]<<24);
	t->pos += 4;

	return res;
}

/*--------------------------------------------------------------------------------*
 * Reads SWF to memory
 *--------------------------------------------------------------------------------*/
int swf_ReadSWF2(reader_t* reader, SWF* swf)
{
char b[32];
int len;
int tc;  /* tag counter */
TAG* t;
TAG t1;
reader_t zreader;

	if(NULL == swf) return -1;

	memset(swf, 0x00, sizeof(SWF));

	len = reader->read(reader, b, 8);
	if(len < 8) return -1;

	if(b[0] != 'F' && b[0] != 'C') return -1;
	if(b[1] != 'W') return -1;
	if(b[2] != 'S') return -1;
	swf->fileVersion = b[3];
	swf->compressed = (b[0]=='C') ? 1 : 0;
	swf->fileSize = GET32(&b[4]);

	if(swf->compressed)
	{
		/* at first, we do not care about SWC */
		return -1;
	}
	swf->compressed = 0;
	reader_GetRect(reader, &swf->movieSize);
	reader->read(reader, &swf->frameRate, 2);
	swf->frameRate = LE_16_TO_NATIVE(swf->frameRate);
	reader->read(reader, &swf->frameCount, 2);
	swf->frameCount = LE_16_TO_NATIVE(swf->frameCount);

	/* read tags and connect to list */
	t1.next = 0;
	t = &t1;
	tc = 0;
	while (t)
	{
		t = swf_ReadTag(reader, t);
		if(NULL == t) break;
		tc++;
		if(t->id == ST_FILEATTRIBUTES)
		{
			swf->fileAttributes = swf_GetU32(t);
			swf_ResetReadBits(t);
		}
	}
/*
	printf("swf->frameCount:%d <--> tag count:%d\n", swf->frameCount, tc);
*/
	swf->firstTag = t1.next;
	if(t1.next)
		t1.next->prev = NULL;
	
	return reader->pos;
}


int swf_ReadSWF(int handle, SWF* swf)
{
  reader_t reader;
  reader_init_filereader(&reader, handle);

  return swf_ReadSWF2(&reader, swf);
}

int swf_FreeTags(SWF* swf)
{
int tag_counter;
TAG* t;
TAG* tnew;

	t = swf->firstTag;

	tag_counter = 0;
	while(t)
	{
		tag_counter++;
		tnew = t->next;
		if(t->data) rfx_free(t->data);
		rfx_free(t);
		t = tnew;
	}
	swf->firstTag = 0;

	return tag_counter;
}
