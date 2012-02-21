
/*
参数值：
    whole：输入整体字符串, 例如："c:/system32/abc.txt"
    dir: 返回路径名，例如：c:/system32

返回值：
    dir路径结果的长度，例如，本例中为11。
*/
int GetDir(char *whole,char* dir);


/*
参数值：
    whole：输入整体字符串, 例如："c:/system32/abc.txt"
    name: 返回文件名，例如：abc

返回值：
    文件名结果的长度，例如，本例中为3。
*/
int GetName(char *whole,char* name);


/*
参数值：
    whole：输入整体字符串, 例如："c:/system32/abc.txt"
    ext: 返回文件名后缀，例如：txt

返回值：
    文件名后缀结果的长度，例如，本例中为3。
*/
int GetExt(char *whole,char* ext);


/*
remarks: you input oldFileName="demo.txt", and part="ch".The NewFileName will be "demo-ch.txt".
it will insert string "ch" into the oldFileName between FileName and extension.
*/
void InsertPartToFile(char *oldFileName,char* part,char* NewFileName);

void TestFileNameTools(void);
