#include<stdio.h>
#include<stdlib.h>
#include "myargs.h"
#include "FileNameTools.h"
#include "TxtTools.h"

void main(int argc,char* argv[])
{
	//TestFileNameTools();
	//In the beginning of main, Parse the argv.
	
	if(ParseMyArgs(argc,argv)==1)
	{
		exit(1);
	}

	if(i->count>0)
	{
		TestTxtTools();
	}

	if(size->count >0)
	{
		printf("size=%d\n",size->ival[0]);
	}

	if(keyword->count >0)
	{
		printf("keyword=%s",keyword->sval[0]);
	}

	for (int j=0; j<files->count; j++)
        printf("filename[%d] = \"%s\"\n", j, files->filename[j]);


	//TestTxtTools();
	//getch(); /*可以使得输出窗口停住，让你检查一下最后的输出是否正确*/

	//in the end of main, call Delete MyArgs
	DeleteMyArgs();
}
