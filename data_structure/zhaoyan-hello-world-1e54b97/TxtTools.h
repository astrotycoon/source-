#include <string>
using namespace std;


/*
参数值：
    输入文件名
返回值：
    返回文件中的行数，以回车为行分隔符。
*/
int CountLines(char* FileName);



/*
参数值：
    输入文件名
返回值：
    返回文件中的单词数（针对英文文件，以空格为词分割单位）
*/
int CountEnglishWords(char* FileName);



/*
作用：
		把源文件中的空行去掉，结果保存在目标文件名制定的文件中。
参数值：
    源文件名和目标文件名
返回值：
    空行数
*/
int DelBlankLines(char* srcFileName,char* desFileName);


/*
作用：
		Insert "Chapter 1: " into the file according to content of file. 
		This function can be used to format txt book for Stanza reader, 
		which is the most popular reader in IPhone. 
参数值：
    源文件名, 目标文件名, name of book, author of book
返回值：
*/
int InsertTagToFile(char* srcFileName,char* desFileName,char* bookTitle,char* bookAuthor);

/*
作用：
		Split a big fine into some small file according to its content
参数值：
    源文件名和目标文件名
返回值：
*/
int SplitFile(char* srcFileName,char* desFileName);

/*
Remarks:
	Delete the space, tab in the two sides of the strSrc, and newline in the end of strSrc,
and copy the result to strDes

Return Value:
	the length of strDes

*/
string Trim(const string & AStr);

/*
作用：
		实验用函数
*/
void TestTxtTools(void);
