#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <map>
#define BUF_SIZE 2048  // 输入缓存区大小
#define KEY_NUM 28  // 关键字数量
using namespace std;

// 储存记号流中每个词以及相关信息
typedef struct all_words
{
    string sign;  // 记号 
    string attribute;  // 属性
}ALL_WORDS;

// 记录程序中的错误
typedef struct all_errors
{
    string errorInf;  // 记录错误信息
	string errorToken;  // 记录错误token
    int lines;  // 记录错误行数
}ALL_ERRORS;

// 关键字
string key[28] = 
{ "int", "long", "short", "float", "double", "char", "unsigned", "signed", "const", "void", "struct", "union", 
"if", "else", "goto", "switch", "case", "do", "while", "for", "continue", "break", "return", "default", "typedef", 
"extern", "static", "main" };

vector<string> IDs;  // 识别出的符号表 
vector<all_words> wordFlow;  // 识别出的记号流
vector<ALL_ERRORS> errorFlow;  // 识别出的错误
string FILE_NAME = "test.txt";  // 默认测试文件
FILE *filePtr;  // 打开的文件指针
ofstream fout; // 输出文件流
char buf[BUF_SIZE];  // 输入缓存(分为左右两半)
int forwardPtr;  // 缓存区前向指针
int validChNum = 0;  // 有效字符数(不含空格，换行等)
int allChNum = 0;  // 所有字符数(含空格，换行等)
int allRows = 0;  // 总行数
int nowRows = 0;  // 当前行数



int get_buf(int part)  // 返回读取的字符数
{
    char tmp = '\0';
    int i = 0;
    int offset = 0;  // buf偏移量

    if (part == 0)  // 获取右半区
        offset = BUF_SIZE / 2;
    else  // 获取左半区
        offset = 0;

    for (i = 0; i < BUF_SIZE / 2; i++)
    {
        tmp = fgetc(filePtr);
        if (tmp == EOF)  // 判断文件是否结束
        {
            break;
        }
        else
        {
            buf[offset + i] = tmp;
			//cout << buf[offset + i] << "  " << endl;//test code
            if (tmp == '\n')  // 如果是换行，则行数计数加一
                allRows++;
            if (tmp != ' ' && tmp != '\t' && tmp != '\n')  // 如果是有效字符，则有效字符计数加一
            {
                validChNum++;
            }
        }
    }

    allChNum += i;
    return i;
}

void get_char(char& C)  // 根据forawrdPtr的指示获取一个字符C并移动forwardPtr
{
    C = buf[forwardPtr];
	if (C == '\n')
		nowRows++;

    if (forwardPtr == BUF_SIZE / 2)
        get_buf(1);  // 获取右半区
    else if (forwardPtr == BUF_SIZE - 1)
        get_buf(0);  // 获取左半区

    forwardPtr = (forwardPtr + 1) % BUF_SIZE;
}

int iskey(const string token)
{
    int i;
    for (i = 0; i < KEY_NUM; i++)
    {
        if (token == key[i])
            return i;  // 返回key的序号
    }
    return -1;  // 不是key
}

void get_nbc(char& C)  // 跳过空白
{
	while (C == ' ' || C == '\t' || C == '\n')
        get_char(C);
}

void cat(string& token, const char C)  // 把C拼接在token后
{
    token = token + C;
}

bool isLetter(const char C)  // 判断是否为字母
{
    if ((C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z'))
        return true;
    return false;
}

bool isDigit(const char C)  // 判断是否为数字
{
    if (C >= '0' && C <= '9')
        return true;
    return false;
}

void retract()  // forwardPtr后退一个字符
{
    forwardPtr = (forwardPtr - 1 + BUF_SIZE) % BUF_SIZE;
}

int inIDs(const string token)  // 查询单词在符号表中的位置(行数)
{
    int i;
    for (i = 0; i < IDs.size(); i++)
    {
        if (token == IDs[i])
            return i + 1;  // 返回行号(从1开始数)
    }
    return -1;  // 不在符号表中(即这是一个新符号)
}

int table_insert(const string token)  // 在符号表中插入单词token
{
    if (inIDs(token) == -1)  // 是新单词
    {
        IDs.push_back(token);
        return IDs.size();
    }
    else
    {
        return inIDs(token);
    }
}

void words_insert(const ALL_WORDS newWord)  // 插入记号流
{
    wordFlow.push_back(newWord);
}

void error(ALL_ERRORS newError)
{
	errorFlow.push_back(newError);
}

int lexical_analysis()  // 词法分析主模块
{
	int state = 0;  // 状态
	char C = buf[0];  // 当前处理的一个字符
	string token = "";  //当前处理的字符串
	ALL_WORDS tmpWord = { "","" };  // 待加入记号流的单词
	ALL_ERRORS tmpError = { "","", 0 };  // 待记录的错误
	while (C != '\0')
	{
		switch (state)
		{
		case 0:  // 初始状态
			token = "";
			get_char(C);
			get_nbc(C);
			if (isLetter(C) || C == '_')
				state = 1;
			else if (isDigit(C))
				state = 2;
			else
			{
				switch (C)
				{
				case '<':state = 8;	break;
				case '>':state = 9; break;
				case '&':state = 10; break;
				case '|':state = 11; break;
				case '+':
				case '-':
				case '*':
				case '%':state = 12; break;
				case '/':state = 13; break;
				case '=':state = 17; break;
				case '!':state = 18; break;
				case '"':state = 19; break;
				case '\'':state = 20; break;
				case '#':state = 21; break;
				case '(':
				case ')':
				case '[':
				case ']':
				case '{':
				case '}':
				case ';':
				case ':':
				case ',':
					state = 0;
					tmpWord.attribute = C;
					tmpWord.sign = "delimi";
					words_insert(tmpWord);
					break;
				case EOF:return 0;
				default:
					state = 22;
					break;
				}
			}
			break;
		case 1:  // 标识符状态
			cat(token, C);
			get_char(C);
			if (isLetter(C) || isDigit(C) || C == '_')
			{
				state = 1;
			}
			else
			{
				retract();
				state = 0;
				if (iskey(token) != -1)
				{
					tmpWord.attribute = key[iskey(token)];
					tmpWord.sign = "";
					words_insert(tmpWord);
				}
				else
				{
					if (isDigit(token[0]))
					{
						tmpError.errorInf = "Identifier starts with a number";
						tmpError.errorToken = token;
						tmpError.lines = nowRows;
						error(tmpError);
					}
					else
					{
						tmpWord.attribute = token;
						tmpWord.sign = "id";
						words_insert(tmpWord);
						table_insert(token);
					}
				}
			}
			break;
		case 2:  // 常数状态
			cat(token, C);
			get_char(C);
			if (isDigit(C))
				state = 2;
			else if ((isLetter(C) && C != 'e' && C != 'E') || C == '_')
				state = 1;
			else
			{
				switch (C)
				{
				case '.':state = 3; break;
				case 'e':
				case 'E':state = 5; break;
				default:
					retract();
					state = 0; 
					tmpWord.attribute = token;
					tmpWord.sign = "num";
					words_insert(tmpWord);
					break;
				}
			}
			break;
		case 3:  // 小数点状态
			cat(token, C);
			get_char(C);
			if (isDigit(C))
				state = 4;
			else if (isLetter(C) || C == '_')
				state = 1;
			else
			{
				state = 0;
				tmpError.errorInf = "Missing content after the decimal point";
				tmpError.errorToken = token;
				tmpError.lines = nowRows;
				error(tmpError);
			}
			break;
		case 4:  // 小数状态
			cat(token, C);
			get_char(C);
			if (isDigit(C))
				state = 4;
			else if ((isLetter(C) && C != 'e' && C != 'E') || C == '_')
				state = 1;
			else
			{
				switch (C)
				{
				case 'e':
				case 'E':state = 5; break;
				default:
					retract();
					state = 0;
					tmpWord.attribute = token;
					tmpWord.sign = "num";
					words_insert(tmpWord);
					break;
				}
			}
			break;
		case 5:  // 指数状态
			cat(token, C);
			get_char(C);
			if (isDigit(C))
				state = 7;
			else if (isLetter(C) || C == '_')
				state = 1;
			else
			{
				switch (C)
				{
				case '+':
				case '-':state = 6; break;
				default:
					retract();
					state = 0; 
					tmpError.errorInf = "Missing sign after exponent";
					tmpError.errorToken = token;
					tmpError.lines = nowRows;
					error(tmpError);
					break;
				}
			}
			break;
		case 6:
			cat(token, C);
			get_char(C);
			if (isDigit(C))
				state = 7;
			else if (isLetter(C) || C == '_')
				state = 1;
			else
			{
				retract();
				state = 0;
				tmpError.errorInf = "The number after the index is illegal";
				tmpError.errorToken = token;
				tmpError.lines = nowRows;
				error(tmpError);
			}
			break;
		case 7:
			cat(token, C);
			get_char(C);
			if (isDigit(C))
				state = 7;
			else if (isLetter(C) || C == '_')
				state = 1;
			else
			{
				retract();
				state = 0; 
				tmpWord.attribute = token;
				tmpWord.sign = "num";
				words_insert(tmpWord);
			}
			break;
		case 8:  // '<'状态
			cat(token, C);
			get_char(C);
			switch (C)
			{
			case '<':
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "shift";
				words_insert(tmpWord);
				break;
			case '=':
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "relop";
				words_insert(tmpWord);
				break;
			default:
				retract();
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "relop";
				words_insert(tmpWord);
				break;
			}
			break;
		case 9:  // '>'状态
			cat(token, C);
			get_char(C);
			switch (C)
			{
			case '>':
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "shift";
				words_insert(tmpWord);
				break;
			case '=':
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "relop";
				words_insert(tmpWord);
				break;
			default:
				retract();
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "relop";
				words_insert(tmpWord);
				break;
			}
			break;
		case 10:  // '&'状态
			cat(token, C);
			get_char(C);
			if (C == '&')
			{
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "logic";
				words_insert(tmpWord);
			}
			else
			{
				retract();
				state = 0;
				tmpWord.attribute = "addre";
				tmpWord.sign = token;
				words_insert(tmpWord);
			}
			break;
		case 11:  // '|'状态
			cat(token, C);
			get_char(C);
			if (C == '|')
			{
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "logic";
				words_insert(tmpWord);
			}
			else
			{
				retract();
				state = 0;
				tmpError.errorInf = "\"||\" is incomplete";
				tmpError.errorToken = token;
				tmpError.lines = nowRows;
				error(tmpError);
			}
			break;
		case 12:  // '+' '-' '*' '%'状态
			cat(token, C);
			get_char(C);
			if (C == '=')
			{
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "assin";
				words_insert(tmpWord);
			}
			else
			{
				retract();
				state = 0; 
				tmpWord.attribute = token;
				tmpWord.sign = "opera";
				words_insert(tmpWord);
			}
			break;
		case 13:  // '/'状态
			cat(token, C);
			get_char(C);
			switch (C)
			{
			case '=': 
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "assin";
				words_insert(tmpWord);
				break;
			case '/':state = 14; break;
			case '*':state = 15; break;
			default:
				retract();
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "opera";
				words_insert(tmpWord);
				break;
			}
			break;
		case 14:  // "//"状态(单行注释状态)
			cat(token, C);
			get_char(C);
			if (C == '\n')
			{
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "annota";
				words_insert(tmpWord);
			}
			else
			{
				state = 14;
			}
			break;
		case 15:  // "/*状态(多行注释状态)
			cat(token, C);
			get_char(C);
			if (C == '*')
			{
				state = 16;
			}
			else
			{
				state = 15;
			}
			break;
		case 16:  // "/*...*"状态(等待完整的"*/")
			cat(token, C);
			get_char(C);
			if (C == '*')
			{
				state = 16;
			}
			else if (C == '/')
			{
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "annota";
				words_insert(tmpWord);
			}
			else
			{
				state = 15;
			}
			break;
		case 17:  // '='状态
			cat(token, C);
			get_char(C);
			if (C == '=')
			{
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "relop";
				words_insert(tmpWord);
			}
			else
			{
				retract();
				state = 0; 
				tmpWord.attribute = token;
				tmpWord.sign = "assin";
				words_insert(tmpWord);
			}
			break;
		case 18:  // '!'状态
			cat(token, C);
			get_char(C);
			if (C == '=')
			{
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "relop";
				words_insert(tmpWord);
			}
			else
			{
				retract();
				state = 0;
				tmpError.errorInf = "\"!=\" is incomplete";
				tmpError.errorToken = token;
				tmpError.lines = nowRows;
				error(tmpError);
			}
			break;
		case 19:  // '"'状态
			cat(token, C);
			get_char(C);
			if (C == '"')
			{
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "str";
				words_insert(tmpWord);
			}
			else
			{
				state = 19;
			}
			break;
		case 20:  // '''状态
			cat(token, C);
			get_char(C);
			if (C == '\'')
			{
				cat(token, C);
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "char";
				words_insert(tmpWord);
			}
			else
			{
				state = 20;
			}
			break;
		case 21:  // '#'状态
			cat(token, C);
			get_char(C);
			if (C == '\n')
			{
				state = 0;
				tmpWord.attribute = token;
				tmpWord.sign = "pretrt";
				words_insert(tmpWord);
			}
			else
			{
				state = 21;
			}
			break;
		case 22:  // 错误状态
			state = 0;
			cat(token, C);
			tmpError.errorInf = "Unrecognized symbol";
			tmpError.errorToken = token;
			tmpError.lines = nowRows;
			error(tmpError);
			break;
		default:
			cout << "STATE ERROR!" << endl;
			exit(-1);
		}
	}
}

int lexInit()  // 初始化模块
{
	fout.open("output.txt");
	filePtr = fopen(FILE_NAME.c_str(), "r");
	if (!filePtr)
	{
		printf("Error in opening file");
		return -1;  // 初始化失败
	}

	int i;
	for (i = 0; i < BUF_SIZE; i++)
		buf[i] = '\0';


	forwardPtr = 0;
	validChNum = 0;
	allChNum = 0;
	allRows = 0;
	nowRows = 1;
	get_buf(1);
	return 1;  // 初始化成功
}

void print_result()  // 输出模块
{
	int i = 0;

	cout << "Token stream:" << wordFlow.size() << endl;
	cout << "sign\t\tattribute" << endl;
	fout << "Token stream:" << wordFlow.size() << endl;
	fout << "sign\t\tattribute" << endl;
	for (i = 0; i < wordFlow.size(); i++)
	{
		cout << wordFlow[i].sign << "\t\t" << wordFlow[i].attribute << endl;
		fout << wordFlow[i].sign << "\t\t" << wordFlow[i].attribute << endl;
	}
	cout << endl;
	fout << endl;

	cout << "Symbol table:"<< IDs.size() << endl;
	fout << "Symbol table:" << IDs.size() << endl;
	for (i = 0; i < IDs.size(); i++)
	{
		cout << IDs[i] << endl;
		fout << IDs[i] << endl;
	}
	cout << endl;
	fout << endl;

	cout << "Errors:"<< errorFlow.size() << endl;
	cout << "line\t\ttoken\t\tInf" << endl;
	fout << "Errors:" << errorFlow.size() << endl;
	fout << "line\t\ttokem\t\tInf" << endl;
	for (i = 0; i < errorFlow.size(); i++)
	{
		cout << errorFlow[i].lines << "\t\t" << errorFlow[i].errorToken << "\t\t" << errorFlow[i].errorInf << endl;
		fout << errorFlow[i].lines << "\t\t" << errorFlow[i].errorToken << "\t\t" << errorFlow[i].errorInf << endl;
	}
	cout << endl;
	fout << endl;

	cout << "Total number of rows: " << allRows << endl;
	cout << "Total characters: " << allChNum << endl;
	cout << "Total valid characters: " << validChNum << endl;
	cout << "Total words: " << IDs.size() << endl; 
	fout << "Total number of rows: " << allRows << endl;
	fout << "Total characters: " << allChNum << endl;
	fout << "Total valid characters: " << validChNum << endl;
	fout << "Total words: " << IDs.size() << endl;
}

int main()  // 主函数
{
	// 获取文件名
	cout << "Please enter the file name to be analyzed: ";
	cin >> FILE_NAME;

	if (lexInit())
	{
		lexical_analysis();  // 开始词法分析
		print_result();  // 输出分析结果
		fclose(filePtr);  // 关闭文件
		fout.close();
	}
	else  // 初始化失败
		cout << "Initialization failed!" << endl;

	return 0;
}