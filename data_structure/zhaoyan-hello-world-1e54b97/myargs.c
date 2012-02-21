
#include <stdio.h>
#include "myargs.h"


struct arg_lit *i, *s, *author;
struct arg_str *keyword;
struct arg_int *size;
struct arg_lit *help,*version;
struct arg_file *files;
struct arg_end *end;


void *argtable[8];


int ParseMyArgs(int argc, char *argv[])
{
	int exitcode=0;
    int nerrors;

	argtable[0] =i= arg_lit0("i", "insert",           "insert a tag into the text file");
	argtable[1]=s= arg_lit0("s", "separate",          "separate a text file into smaller parts");
	               arg_rem("NULL",                    "new files with number suffix");
    argtable[2] =author= arg_lit0(NULL,"author",              "print the author of each file");
	argtable[3]=keyword= arg_str0(NULL, "keyword", "WORD",          "keyword to seperate the parts in ");
						 arg_rem(NULL,                            " a text book");
    argtable[4] =size= arg_int0(NULL,"size", "SIZE",              "size of each of parts");
	argtable[5] =help= arg_lit0(NULL,"help",                "display this help and exit");
	argtable[6] =version= arg_lit0(NULL,"version",             "display version information and exit");
	argtable[7] = files = arg_filen(NULL, NULL, "FILE", 0, argc+2, NULL),
	argtable[8] = end= arg_end(20);

	
 /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
        {
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n",argv[0]);
        exitcode=1;
        goto exit;
        }

    
    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);
	if(author->count >0)
	{
		printf("This software is developed by a teacher and some students\n");
		printf("Detail can be found in README file");
	}

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
        {
        printf("Usage: %s", argv[0]);
        arg_print_syntax(stdout,argtable,"\n");
		 arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        exitcode=1;
        goto exit;
        } 

    /* special case: '--version' takes precedence error reporting */
    if (version->count > 0){
        printf("Dec 2010, \n");
        exitcode=1;
        goto exit;
        } 

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
        {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout,end,argv[0]);
        printf("Try '%s --help' for more information.\n",argv[0]);
        exitcode=1;
        goto exit;
        }

    /* Command line parsing is complete, do the main processing */

exit:   
    return exitcode;
}

void DeleteMyArgs(void)
{
	 /* deallocate each non-null entry in argtable[] */
	arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}
