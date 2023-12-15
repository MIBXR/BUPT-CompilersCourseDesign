#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <map>
#define BUF_SIZE 2048  // ���뻺������С
#define KEY_NUM 28  // �ؼ�������
using namespace std;

// ����Ǻ�����ÿ�����Լ������Ϣ
typedef struct all_words
{
    string sign;  // �Ǻ� 
    string attribute;  // ����
}ALL_WORDS;

// ��¼�����еĴ���
typedef struct all_errors
{
    string errorInf;  // ��¼������Ϣ
	string errorToken;  // ��¼����token
    int lines;  // ��¼��������
}ALL_ERRORS;

// �ؼ���
string key[28] = 
{ "int", "long", "short", "float", "double", "char", "unsigned", "signed", "const", "void", "struct", "union", 
"if", "else", "goto", "switch", "case", "do", "while", "for", "continue", "break", "return", "default", "typedef", 
"extern", "static", "main" };

vector<string> IDs;  // ʶ����ķ��ű� 
vector<all_words> wordFlow;  // ʶ����ļǺ���
vector<ALL_ERRORS> errorFlow;  // ʶ����Ĵ���
string FILE_NAME = "test.txt";  // Ĭ�ϲ����ļ�
FILE *filePtr;  // �򿪵��ļ�ָ��
ofstream fout; // ����ļ���
char buf[BUF_SIZE];  // ���뻺��(��Ϊ��������)
int forwardPtr;  // ������ǰ��ָ��
int validChNum = 0;  // ��Ч�ַ���(�����ո񣬻��е�)
int allChNum = 0;  // �����ַ���(���ո񣬻��е�)
int allRows = 0;  // ������
int nowRows = 0;  // ��ǰ����



int get_buf(int part)  // ���ض�ȡ���ַ���
{
    char tmp = '\0';
    int i = 0;
    int offset = 0;  // bufƫ����

    if (part == 0)  // ��ȡ�Ұ���
        offset = BUF_SIZE / 2;
    else  // ��ȡ�����
        offset = 0;

    for (i = 0; i < BUF_SIZE / 2; i++)
    {
        tmp = fgetc(filePtr);
        if (tmp == EOF)  // �ж��ļ��Ƿ����
        {
            break;
        }
        else
        {
            buf[offset + i] = tmp;
			//cout << buf[offset + i] << "  " << endl;//test code
            if (tmp == '\n')  // ����ǻ��У�������������һ
                allRows++;
            if (tmp != ' ' && tmp != '\t' && tmp != '\n')  // �������Ч�ַ�������Ч�ַ�������һ
            {
                validChNum++;
            }
        }
    }

    allChNum += i;
    return i;
}

void get_char(char& C)  // ����forawrdPtr��ָʾ��ȡһ���ַ�C���ƶ�forwardPtr
{
    C = buf[forwardPtr];
	if (C == '\n')
		nowRows++;

    if (forwardPtr == BUF_SIZE / 2)
        get_buf(1);  // ��ȡ�Ұ���
    else if (forwardPtr == BUF_SIZE - 1)
        get_buf(0);  // ��ȡ�����

    forwardPtr = (forwardPtr + 1) % BUF_SIZE;
}

int iskey(const string token)
{
    int i;
    for (i = 0; i < KEY_NUM; i++)
    {
        if (token == key[i])
            return i;  // ����key�����
    }
    return -1;  // ����key
}

void get_nbc(char& C)  // �����հ�
{
	while (C == ' ' || C == '\t' || C == '\n')
        get_char(C);
}

void cat(string& token, const char C)  // ��Cƴ����token��
{
    token = token + C;
}

bool isLetter(const char C)  // �ж��Ƿ�Ϊ��ĸ
{
    if ((C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z'))
        return true;
    return false;
}

bool isDigit(const char C)  // �ж��Ƿ�Ϊ����
{
    if (C >= '0' && C <= '9')
        return true;
    return false;
}

void retract()  // forwardPtr����һ���ַ�
{
    forwardPtr = (forwardPtr - 1 + BUF_SIZE) % BUF_SIZE;
}

int inIDs(const string token)  // ��ѯ�����ڷ��ű��е�λ��(����)
{
    int i;
    for (i = 0; i < IDs.size(); i++)
    {
        if (token == IDs[i])
            return i + 1;  // �����к�(��1��ʼ��)
    }
    return -1;  // ���ڷ��ű���(������һ���·���)
}

int table_insert(const string token)  // �ڷ��ű��в��뵥��token
{
    if (inIDs(token) == -1)  // ���µ���
    {
        IDs.push_back(token);
        return IDs.size();
    }
    else
    {
        return inIDs(token);
    }
}

void words_insert(const ALL_WORDS newWord)  // ����Ǻ���
{
    wordFlow.push_back(newWord);
}

void error(ALL_ERRORS newError)
{
	errorFlow.push_back(newError);
}

int lexical_analysis()  // �ʷ�������ģ��
{
	int state = 0;  // ״̬
	char C = buf[0];  // ��ǰ�����һ���ַ�
	string token = "";  //��ǰ������ַ���
	ALL_WORDS tmpWord = { "","" };  // ������Ǻ����ĵ���
	ALL_ERRORS tmpError = { "","", 0 };  // ����¼�Ĵ���
	while (C != '\0')
	{
		switch (state)
		{
		case 0:  // ��ʼ״̬
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
		case 1:  // ��ʶ��״̬
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
		case 2:  // ����״̬
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
		case 3:  // С����״̬
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
		case 4:  // С��״̬
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
		case 5:  // ָ��״̬
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
		case 8:  // '<'״̬
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
		case 9:  // '>'״̬
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
		case 10:  // '&'״̬
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
		case 11:  // '|'״̬
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
		case 12:  // '+' '-' '*' '%'״̬
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
		case 13:  // '/'״̬
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
		case 14:  // "//"״̬(����ע��״̬)
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
		case 15:  // "/*״̬(����ע��״̬)
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
		case 16:  // "/*...*"״̬(�ȴ�������"*/")
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
		case 17:  // '='״̬
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
		case 18:  // '!'״̬
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
		case 19:  // '"'״̬
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
		case 20:  // '''״̬
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
		case 21:  // '#'״̬
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
		case 22:  // ����״̬
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

int lexInit()  // ��ʼ��ģ��
{
	fout.open("output.txt");
	filePtr = fopen(FILE_NAME.c_str(), "r");
	if (!filePtr)
	{
		printf("Error in opening file");
		return -1;  // ��ʼ��ʧ��
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
	return 1;  // ��ʼ���ɹ�
}

void print_result()  // ���ģ��
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

int main()  // ������
{
	// ��ȡ�ļ���
	cout << "Please enter the file name to be analyzed: ";
	cin >> FILE_NAME;

	if (lexInit())
	{
		lexical_analysis();  // ��ʼ�ʷ�����
		print_result();  // ����������
		fclose(filePtr);  // �ر��ļ�
		fout.close();
	}
	else  // ��ʼ��ʧ��
		cout << "Initialization failed!" << endl;

	return 0;
}