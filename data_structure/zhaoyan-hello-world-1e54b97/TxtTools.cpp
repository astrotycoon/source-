#include "TxtTools.h"
#include <stdio.h>
#include "FileNameTools.h"
#include "myargs.h"

#define MAX_LINE 10000 /* you can use define here, but const int in c++ is better*/
#define MAX_WORD 100

int CountLines(char* FileName){ 
	int lineNum=0;
	char line[MAX_LINE];
	FILE* fh;

	fh = fopen(FileName,"r");
	if(fh == NULL) /* judge fh must followed fopen, it's a RULE!! */
	{
		fprintf(stderr, "open file %s fail\n",FileName); /* output message to stderr */
		return -1;
	}

	while(fgets(line,MAX_LINE,fh)!=NULL) /* it's code snippet common used, you need remember it to read every line from a file */
	{
		/*fputs(line,stdout);*/ /*or use printf("%s",line); if the file is very big, comment this statement to save time*/
		lineNum++;
	}
	fclose(fh);
	return lineNum;
}


int CountEnglishWords(char* FileName){

   int wordNum=0;
   char word[MAX_WORD];
   FILE *fp;

   fp = fopen(FileName,"r");
   if(fp == NULL){
     fprintf(stderr,"open file %s fail\n",FileName);
     return -1;
   }
   while(fscanf(fp,"%s",word)!=EOF){ /* EOF is End of File */
     wordNum++;
   }
   fclose(fp);
   return wordNum;
  
 }


int DelBlankLines(char* srcFileName,char* desFileName){
  int lineNum=0;
  char line[MAX_LINE];
  FILE *fpRead,*fpWrite;

  fpRead = fopen(srcFileName,"r");
  fpWrite = fopen(desFileName,"w");
  if (fpRead == NULL){
    fprintf(stderr,"open file %s fail\n",srcFileName);
    return -1;
  }

  if (fpWrite == NULL){
    fprintf(stderr,"open file %s fail\n",desFileName);
    return -1;
  }

  while(fgets(line,MAX_LINE,fpRead)!=NULL){
    if (line[0]!='\n') {
      lineNum++;
      fputs(line,fpWrite);
    }
  }
  fclose(fpWrite);
  fclose(fpRead);
  return lineNum;
}

string Trim(const string & AStr)
{
	//清除两侧<边界符>：空格，水平制表符，垂直制表符，换行，回车符
	size_t mLen = AStr.length();
	if (mLen == 0) return "";
	int mbegin, mend ;

	for (mbegin = 0 ; mbegin < mLen ; mbegin ++)  //
	{
		const char & mch = AStr[mbegin];
		if ((mch != ' ') && (mch != '\n') 
		   && (mch != '\r') && (mch != '\t') && (mch != '\v')) break;
	}
    if (mbegin == mLen) return "";  //全部是边界符，（jw）

	for (mend = mLen - 1 ; mend >= 0 ; mend --)  //
	{
		const char & mch = AStr[mend];
		if ((mch != ' ') && (mch != '\n') 
		   && (mch != '\r') && (mch != '\t') && (mch != '\v')) break;
	}

	if (mend < 0) return "";  //全部是边界符，该情况，不会出现，因为，如果出现则（jw）处一定退出了

	return AStr.substr(mbegin, mend - mbegin + 1);
}


int InsertTagToFile(char* srcFileName,char* desFileName,char* bookTitle,char* bookAuthor)
{
	FILE *scrFileP,*desFileP; /* name of scrFileP is better than name of scrFileNameP, why ? */
	char line[MAX_LINE]; /* don't use char Line[10000], 10000 is magic number,magic number is bad style */
	char partLine[30];
	char desName[255];
	char desExt[255];
	char num[10]; 
	char di[3] = "第";
	char hui[3] = "回";
	char tag[50] = "Chapter ";
	char title[50] = "title";
	char *presult ;

	int sectionNum=0; 
	int lineNum = 0;

	lineNum = CountLines(srcFileName); /* CountLines is a function which we wrote before, use it here :) */
	printf("There are %d lines in the %s file",lineNum,srcFileName);

	scrFileP = fopen(srcFileName,"r");
	desFileP = fopen(desFileName,"w");
	GetName(desFileName,desName); /* get the name and extension of desFileName */
	GetExt(desFileName,desExt);
	
	if (scrFileP == NULL){  /* This judgement is a code snippet, to test if a file is opened correctly*/
		fprintf(stderr,"open file %s fail\n",srcFileName);
		return -1;
	}

	if (desFileP == NULL){
		fprintf(stderr,"open file %s fail\n",desFileName);
		return -1;
	}
	fprintf(desFileP,"%s","Title: ");
	fprintf(desFileP,"%s\n",bookTitle);
	fprintf(desFileP,"%s","Author: ");
	fprintf(desFileP,"%s\n",bookAuthor);

	while(fgets(line,MAX_LINE,scrFileP)!=NULL){
		strncpy(partLine,line,30);
		partLine[29]='\0'; /* why I add zero here ?*/
		if(strstr(partLine,di) && strstr(partLine,hui)){ /* to find '第' and '回' int the first 20 characters. */
			
			presult = strstr(partLine,hui);
			if(presult != NULL)
			{
				strcpy(title,presult+2);
				printf("%s",title);
			}
			sectionNum++;
			sprintf(num,"%d",sectionNum); /* change number to string */
			strcpy(tag,"Chapter ");
			strcat(tag,num);
			strcat(tag,": ");
			strcat(tag,title);
			fprintf(desFileP,"\n\n%s\n\n",tag);
        }
    fprintf(desFileP,"%s\n",line);
  }
  fclose(scrFileP); /* close files inthe end*/
  fclose(desFileP);
}

int SplitFile(char* srcFileName,char* desFileName)
{
	FILE *scrFileP,*desFileP; /* name of scrFileP is better than name of scrFileNameP, why ? */
	char line[MAX_LINE]; /* don't use char Line[10000], 10000 is magic number,magic number is bad style */
	char partLine[30];
	char desName[255];
	char desExt[255];
	char num[10]; 
	char di[3] = "第";
	char hui[3] = "回";
	char tag[50] = "Chapter ";
	char title[50] = "title";
	char *presult ;

	int sectionNum=0; 
	int lineNum = 0;

	lineNum = CountLines(srcFileName); /* CountLines is a function which we wrote before, use it here :) */
	printf("There are %d lines in the %s file",lineNum,srcFileName);

	scrFileP = fopen(srcFileName,"r");
	desFileP = fopen(desFileName,"w");
	GetName(desFileName,desName); /* get the name and extension of desFileName */
	GetExt(desFileName,desExt);
	
	if (scrFileP == NULL){  /* This judgement is a code snippet, to test if a file is opened correctly*/
		fprintf(stderr,"open file %s fail\n",srcFileName);
		return -1;
	}

	if (desFileP == NULL){
		fprintf(stderr,"open file %s fail\n",desFileName);
		return -1;
	}

	while(fgets(line,MAX_LINE,scrFileP)!=NULL)
	{
		strncpy(partLine,line,30);
		partLine[29]='\0'; /* why I add zero here ?*/
		if(strstr(partLine,di) && strstr(partLine,hui)) /* to find '第' and '回' int the first 20 characters. */
		{ 
			fclose(desFileP);
			sectionNum++;
			sprintf(num,"%d",sectionNum); /* change number to string */

			GetName(desFileName,desName);
			strcat(desName,"-");
			strcat(desName,num);
			strcat(desName,".");
			strcat(desName,desExt);
			desFileP = fopen(desName,"w");
        }
    fprintf(desFileP,"%s\n",line);
  }
  fclose(scrFileP); /* close files inthe end*/
  fclose(desFileP);
}




void TestTxtTools(void)
{

	char newFileName[250];
	char oldFileName[250];
	char author[250];
	char title[250];
	strcpy(oldFileName,files->filename[0]);
	if(files->count >= 2)
		strcpy(title,files->filename[1]);
	if(files->count == 3)
		strcpy(author,files->filename[2]);

	if(i->count >0)
	{
		InsertPartToFile(oldFileName,"ch",newFileName);
		InsertTagToFile(oldFileName,newFileName,title,author);
	}
	return ;


	/*char * FileName = "test.txt";
	char * NewFileName = "test_new.txt";
	unsigned int numLines;
	unsigned int numWords;
	numLines = CountLines(FileName);
	printf("The line number in the %s is %d\n",FileName, numLines);
	
	numWords = CountEnglishWords(FileName);
	printf("The word number in the %s is %d\n",FileName, numWords);

	DelBlankLines(FileName,NewFileName);*/

}