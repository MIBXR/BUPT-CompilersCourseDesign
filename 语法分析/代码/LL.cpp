#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<stack>

#define TERM_NUM 8				// 终结符数量
#define UNTERM_NUM 5			// 非终结符数量
#define G_NUM 10				// 产生式数量
#define file_path "lexical.txt"

using namespace std;

// 储存记号流中每个词以及相关信息
typedef struct words
{
	char sign;					// 记号 
	string attribute;			// 属性
}WORDS;

// 全局变量定义区
vector<string> G_LEFT;			// 储存产生式左边
vector<string> G_RIGHT;			// 储存产生式右边
vector<string> TERM;			// 储存终结符
vector<string> UNTERM;			// 储存非终结符
vector<string> FIRST[TERM_NUM];	// 所有非终结符的FIRST集
vector<string> FOLLOW[TERM_NUM];// 所有非终结符的FOLLOW集
vector<vector<string>> table;	// LL的预测分析表
vector<WORDS> input;			// 输入流(来自词法分析)

// 判断是否为终结符
int isTerm(string str)
{
	for (int i = 0; i < TERM.size(); i++)
		if (str == TERM[i])
			return 1;
	return 0;
}

// 判断是否为非终结符
int isUnTerm(string str)
{
	for (int i = 0; i < UNTERM.size(); i++)
		if (str == UNTERM[i])
			return 1;
	return 0;
}

// 寻找非终结符所在索引号
int findIndex(string ch)
{
	for (int i = 0; i < UNTERM_NUM; i++)
		if (ch == UNTERM[i])
			return i;
	cout << "ERROR: can't find '" << ch << "' in UnTermin Set" << endl;
	return -1;
}

// 寻找终结符所在索引号
int findIndexT(string ch)
{
	for (int i = 0; i < TERM_NUM; i++)
		if (ch == TERM[i])
			return i;
	cout << "ERROR: can't find '" << ch << "' in Termin Set" << endl;
	return -1;
}

// 初始化
void initProgram()
{
	vector<string> tmp;
	string F[] = { "E", "A", "T", "B", "F" };											// 非终结符
	string T[] = { "+", "-", "*", "/", "(", ")", "d", "$" };							// 终结符
	string LEFT[] = { "E", "A", "A", "A", "T", "B", "B", "B", "F", "F" };				// 左部生成式
	string RIGHT[] = { "TA", "+TA", "-TA", "@", "FB", "*FB", "/FB", "@", "(E)", "d" };  // 右部生成式

	// 初始化table
	for (int i = 0; i < UNTERM_NUM; i++)
	{
		for (int j = 0; j < TERM_NUM; j++)
		{
			tmp.push_back("NULL");
		}
		table.push_back(tmp);
		tmp.clear();
	}

	// 初始化产生式，终结符与非终结符
	for (int i = 0; i < 10; i++)
	{
		G_LEFT.push_back(LEFT[i]);
		G_RIGHT.push_back(RIGHT[i]);
	}
	for (int i = 0; i < TERM_NUM; i++)
	{
		TERM.push_back(T[i]);
	}
	for (int i = 0; i < UNTERM_NUM; i++)
	{
		UNTERM.push_back(F[i]);
	}

	// 初始化FIRST集，FOLLOW集
	FIRST[0].push_back("(");
	FIRST[0].push_back("d");
	FIRST[1].push_back("+");
	FIRST[1].push_back("-");
	FIRST[1].push_back("@");
	FIRST[2].push_back("(");
	FIRST[2].push_back("d");
	FIRST[3].push_back("*");
	FIRST[3].push_back("/");
	FIRST[3].push_back("@");
	FIRST[4].push_back("(");
	FIRST[4].push_back("d");

	FOLLOW[0].push_back("$");
	FOLLOW[0].push_back(")");
	FOLLOW[1].push_back("$");
	FOLLOW[1].push_back(")");
	FOLLOW[2].push_back("+");
	FOLLOW[2].push_back("-");
	FOLLOW[2].push_back("$");
	FOLLOW[2].push_back(")");
	FOLLOW[3].push_back("+");
	FOLLOW[3].push_back("-");
	FOLLOW[3].push_back("$");
	FOLLOW[3].push_back(")");
	FOLLOW[4].push_back("*");
	FOLLOW[4].push_back("/");
	FOLLOW[4].push_back("+");
	FOLLOW[4].push_back("-");
	FOLLOW[4].push_back("$");
	FOLLOW[4].push_back(")");

	return;
}

// 从词法分析程序的输出获取记号流
void getInput()
{
	string str;
	int count = 0;
	WORDS tmpWord;
	ifstream infile;

	// 打开由词法分析输出的文件
	infile.open(file_path);

	// 去除无用符号
	infile >> str;
	infile >> str;

	// 获取记号流数量
	infile >> count;

	// 去除无用符号
	infile >> str;
	infile >> str;

	// 获取记号详细属性与信息
	for (int i = 0; i < count; i++)
	{
		infile >> str;
		if (str == "num")
		{
			tmpWord.sign = 'd';
			infile >> str;
			tmpWord.attribute = str;
		}
		else
		{
			infile >> str;
			tmpWord.sign = str[0];
			tmpWord.attribute = str;
		}
		input.push_back(tmpWord);
	}

	tmpWord.attribute = "$";
	tmpWord.sign = '$';
	input.push_back(tmpWord);

	// 输出得到的记号流(测试用)
	for (int i = 0; i < input.size(); i++)
	{
		cout << input[i].sign << "\t" << input[i].attribute << endl;
	}
}

// 输出栈内数据
void outPutStack(vector<char> stack)
{
	cout << "\tstack: ";
	for (int i = 0; i < stack.size(); i++)
		cout << stack[i];
	cout << endl;
}

// 输出当前输入串
void outPutIn(int ip)
{
	cout << "\tinput: ";
	for (int i = ip; i < input.size(); i++)
		cout << input[i].attribute;
	cout << endl;
}

// 算法4.2——构造分析表
void genTable()
{
	string tmp;
	for (int i = 0; i < G_NUM; i++)
	{
		tmp = G_RIGHT[i][0];
		if (isUnTerm(tmp))  // 如果右部第一个符号为非终结符
		{
			for (int j = 0; j < FIRST[findIndex(tmp)].size(); j++)
			{
				if (FIRST[findIndex(tmp)][j] != "@")
				{
					table[findIndex(G_LEFT[i])][findIndexT(FIRST[findIndex(tmp)][j])] = G_RIGHT[i];
				}
			}
		}
		else if (tmp == "@")  // 如果可致空
		{
			for (int j = 0; j < FOLLOW[findIndex(G_LEFT[i])].size(); j++)
			{
				table[findIndex(G_LEFT[i])][findIndexT(FOLLOW[findIndex(G_LEFT[i])][j])] = G_RIGHT[i];
			}
		}
		else if (isTerm(tmp))  // 如果是终结符
		{
			table[findIndex(G_LEFT[i])][findIndexT(tmp)] = G_RIGHT[i];
		}
	}
}

// 算法4.1——预测分析程序
int LLanalyse()
{
	vector<char> LLstack;
	LLstack.push_back('$');
	LLstack.push_back('E');
	int step = 1;			// 记录分析步骤数
	int ip = 0;				// 记录输入串读到的位置
	int i, j;
	string tmpX;
	string tmpa;

	char X = LLstack.back();
	tmpX.clear();
	tmpX.push_back(X);
	char a = input[ip].sign;
	tmpa.clear();
	tmpa.push_back(a);

	while (X != '$')
	{
		cout << "Step " << step << ": " << endl;
		outPutStack(LLstack);
		outPutIn(ip);
		if (isTerm(tmpX))  // 如果是终结符
		{
			if (X == a)  // 匹配成功，弹栈并继续分析
			{
				cout << "\tPop '" << X << "' from stack" << endl;
				LLstack.pop_back();
				ip++;
			}
			else  //匹配失败，报错并结束分析
			{
				cout << "\tError: can't match '" << X << "' and '" << a << "'." << endl;
				return -1;
			}
		}
		else if (isUnTerm(tmpX))  // 如果是非终结符
		{
			i = findIndex(tmpX);
			j = findIndexT(tmpa);
			if (table[i][j] == "NULL")  // 分析表对应项为空，报错并结束分析
			{
				cout << "\tError: can't find production when '" << X << "' meet '" << a << "'." << endl;
				return -1;
			}
			else
			{
				cout << "\tPop '" << X << "' from stack" << endl;
				cout << "\tMatch production: " << X << " -> " << table[i][j] << endl;
				LLstack.pop_back();
				if (table[i][j] != "@")  // 如果产生式不是空串
				{
					cout << "\tPush '" << table[i][j] << "' reversed" << endl;
					for (int k = table[i][j].size() - 1; k >= 0; k--)
						LLstack.push_back(table[i][j][k]);
				}
			}
		}

		step++;

		// 读取栈顶与输入串第一个字符
		X = LLstack.back();
		tmpX.clear();
		tmpX.push_back(X);
		if (ip < input.size())
		{
			a = input[ip].sign;
			tmpa.clear();
			tmpa.push_back(a);
		}
	}
	if (X == '$' && ip == input.size() - 1)
	{
		cout << "Analyse success" << endl;
	}
	else
	{
		cout << "Analyse fail: stack is empty but input haven't been exhausted" << endl;
	}
}




int main()
{
	initProgram();
	genTable();

	cout << "table: " << endl;
	cout << "+\t\t-\t\t*\t\t/\t\t(\t\t)\t\td\t\t$" << endl;
	for (int i = 0; i < UNTERM_NUM; i++)
	{
		for (int j = 0; j < TERM_NUM; j++)
			cout << UNTERM[i] << " -> " << table[i][j] << "\t";
		cout << endl;
	}
	cout << endl;

	cout << "input stream: " << endl;
	getInput();
	cout << endl;

	cout << "analyse: " << endl;
	LLanalyse();

	return 0;
}