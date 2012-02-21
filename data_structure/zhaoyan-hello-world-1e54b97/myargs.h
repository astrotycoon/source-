#pragma once

#include "argtable2.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
1) five types: lit, int ,str, file and end
2) 
*/
extern struct arg_lit *i, *s, *author;
extern struct arg_str *keyword;
extern struct arg_int *size;
extern struct arg_lit *help,*version;
extern struct arg_file *files;
extern struct arg_end *end;

int ParseMyArgs(int argc, char *argv[]);
void DeleteMyArgs(void);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif
