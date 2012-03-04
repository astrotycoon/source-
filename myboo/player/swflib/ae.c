/*  test program for swflib */
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "swflib.h"

static FILE* mp3file = 0;
static char* filename = "output.mp3";

void handlesoundstream(TAG* tag)
{
	switch(tag->id)
	{
	case ST_SOUNDSTREAMHEAD :
		if((tag->data[1] & 0x30) == 0x20)
		{
			mp3file = fopen(filename, "wb");
		}
		break;
		break;
	case ST_SOUNDSTREAMHEAD2 :
		if((tag->data[1] & 0x30) == 0x20)
		{
			mp3file = fopen(filename, "wb");
		}
		break;
	case ST_SOUNDSTREAMBLOCK :
		if(mp3file)
		{
			fwrite(&tag->data[4], tag->len-4, 1, mp3file);
		}
		break;
	default :
		break;
	}

}
int main(int argc, char* argv[])
{
int f;
int ret;
TAG* tag;
int tagnum;
SWF swf;
	if(argc < 2)
	{
		printf("Usage: %s swfile mp3file\n", argv[0]);
		return 0;
	}

	f = _open(argv[1], _O_RDONLY | _O_BINARY);
	if(f<0)
	{
		printf("Open file error! %s", argv[1]);
		return -1;
	}

	if(argc > 2) filename = argv[2];

	printf("Open file OK! %s\n", argv[1]);

	memset(&swf, 0, sizeof(SWF));
	if( (ret = swf_ReadSWF(f, &swf)) < 0)
	{
		printf("swf_ReadSWF error! :: %s", argv[1]);
		swf_FreeTags(&swf);
		_close(f);
		return -1;
	} 
	_close(f);

	printf("SWF: ---------------[%d]----------------------- \n", ret);
	printf("\tfileVersion:0x%.2x \n \tfileSize:0x%x \n", swf.fileVersion, swf.fileSize);
	printf("\tSRECT: xmin=%d, ymin=%d, xmax=%d, ymax=%d \n", swf.movieSize.xmin,
								 swf.movieSize.ymin,
								 swf.movieSize.xmax,
								 swf.movieSize.ymax);
	printf("\tframeRate: 0x%.4x, frameCount: 0x%.4x\n", swf.frameRate, swf.frameCount);

	tag = swf.firstTag;
	tagnum = 0;

	while(tag)
	{
		if(tag->id == ST_SOUNDSTREAMHEAD || tag->id == ST_SOUNDSTREAMHEAD2 || tag->id == ST_SOUNDSTREAMBLOCK)
		{
			tagnum++;
			handlesoundstream(tag);
		}
		tag = tag->next;
	}

	if(mp3file) 
	{
		printf("\tMP3 Tags: %d\n", tagnum);
		fclose(mp3file);
	}

	ret = swf_FreeTags(&swf);
	printf("\tRelease Tags: %d\n", ret);

	return 0;
}
