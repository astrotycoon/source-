/*  test program for swflib */
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "swflib.h"

int main(int argc, char* argv[])
{
int f;
int ret;
TAG* tag;
int tagnum;
SWF swf;
	if(argc != 2)
	{
		printf("Usage: %s swfile\n", argv[0]);
		return 0;
	}

	f = _open(argv[1], _O_RDONLY | _O_BINARY);
	if(f<0)
	{
		printf("Open file error! %s", argv[1]);
		return -1;
	}

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
		tagnum++;
		tag = tag->next;
	}

	ret = swf_FreeTags(&swf);
	printf("\tRelease Tags: %d\n", ret);

	return 0;
}
