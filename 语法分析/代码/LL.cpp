#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<stack>

#define TERM_NUM 8				// �ս������
#define UNTERM_NUM 5			// ���ս������
#define G_NUM 10				// ����ʽ����
#define file_path "lexical.txt"

using namespace std;

// ����Ǻ�����ÿ�����Լ������Ϣ
typedef struct words
{
	char sign;					// �Ǻ� 
	string attribute;			// ����
}WORDS;

// ȫ�ֱ���������
vector<string> G_LEFT;			// �������ʽ���
vector<string> G_RIGHT;			// �������ʽ�ұ�
vector<string> TERM;			// �����ս��
vector<string> UNTERM;			// ������ս��
vector<string> FIRST[TERM_NUM];	// ���з��ս����FIRST��
vector<string> FOLLOW[TERM_NUM];// ���з��ս����FOLLOW��
vector<vector<string>> table;	// LL��Ԥ�������
vector<WORDS> input;			// ������(���Դʷ�����)

// �ж��Ƿ�Ϊ�ս��
int isTerm(string str)
{
	for (int i = 0; i < TERM.size(); i++)
		if (str == TERM[i])
			return 1;
	return 0;
}

// �ж��Ƿ�Ϊ���ս��
int isUnTerm(string str)
{
	for (int i = 0; i < UNTERM.size(); i++)
		if (str == UNTERM[i])
			return 1;
	return 0;
}

// Ѱ�ҷ��ս������������
int findIndex(string ch)
{
	for (int i = 0; i < UNTERM_NUM; i++)
		if (ch == UNTERM[i])
			return i;
	cout << "ERROR: can't find '" << ch << "' in UnTermin Set" << endl;
	return -1;
}

// Ѱ���ս������������
int findIndexT(string ch)
{
	for (int i = 0; i < TERM_NUM; i++)
		if (ch == TERM[i])
			return i;
	cout << "ERROR: can't find '" << ch << "' in Termin Set" << endl;
	return -1;
}

// ��ʼ��
void initProgram()
{
	vector<string> tmp;
	string F[] = { "E", "A", "T", "B", "F" };											// ���ս��
	string T[] = { "+", "-", "*", "/", "(", ")", "d", "$" };							// �ս��
	string LEFT[] = { "E", "A", "A", "A", "T", "B", "B", "B", "F", "F" };				// ������ʽ
	string RIGHT[] = { "TA", "+TA", "-TA", "@", "FB", "*FB", "/FB", "@", "(E)", "d" };  // �Ҳ�����ʽ

	// ��ʼ��table
	for (int i = 0; i < UNTERM_NUM; i++)
	{
		for (int j = 0; j < TERM_NUM; j++)
		{
			tmp.push_back("NULL");
		}
		table.push_back(tmp);
		tmp.clear();
	}

	// ��ʼ������ʽ���ս������ս��
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

	// ��ʼ��FIRST����FOLLOW��
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

// �Ӵʷ���������������ȡ�Ǻ���
void getInput()
{
	string str;
	int count = 0;
	WORDS tmpWord;
	ifstream infile;

	// ���ɴʷ�����������ļ�
	infile.open(file_path);

	// ȥ�����÷���
	infile >> str;
	infile >> str;

	// ��ȡ�Ǻ�������
	infile >> count;

	// ȥ�����÷���
	infile >> str;
	infile >> str;

	// ��ȡ�Ǻ���ϸ��������Ϣ
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

	// ����õ��ļǺ���(������)
	for (int i = 0; i < input.size(); i++)
	{
		cout << input[i].sign << "\t" << input[i].attribute << endl;
	}
}

// ���ջ������
void outPutStack(vector<char> stack)
{
	cout << "\tstack: ";
	for (int i = 0; i < stack.size(); i++)
		cout << stack[i];
	cout << endl;
}

// �����ǰ���봮
void outPutIn(int ip)
{
	cout << "\tinput: ";
	for (int i = ip; i < input.size(); i++)
		cout << input[i].attribute;
	cout << endl;
}

// �㷨4.2�������������
void genTable()
{
	string tmp;
	for (int i = 0; i < G_NUM; i++)
	{
		tmp = G_RIGHT[i][0];
		if (isUnTerm(tmp))  // ����Ҳ���һ������Ϊ���ս��
		{
			for (int j = 0; j < FIRST[findIndex(tmp)].size(); j++)
			{
				if (FIRST[findIndex(tmp)][j] != "@")
				{
					table[findIndex(G_LEFT[i])][findIndexT(FIRST[findIndex(tmp)][j])] = G_RIGHT[i];
				}
			}
		}
		else if (tmp == "@")  // ������¿�
		{
			for (int j = 0; j < FOLLOW[findIndex(G_LEFT[i])].size(); j++)
			{
				table[findIndex(G_LEFT[i])][findIndexT(FOLLOW[findIndex(G_LEFT[i])][j])] = G_RIGHT[i];
			}
		}
		else if (isTerm(tmp))  // ������ս��
		{
			table[findIndex(G_LEFT[i])][findIndexT(tmp)] = G_RIGHT[i];
		}
	}
}

// �㷨4.1����Ԥ���������
int LLanalyse()
{
	vector<char> LLstack;
	LLstack.push_back('$');
	LLstack.push_back('E');
	int step = 1;			// ��¼����������
	int ip = 0;				// ��¼���봮������λ��
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
		if (isTerm(tmpX))  // ������ս��
		{
			if (X == a)  // ƥ��ɹ�����ջ����������
			{
				cout << "\tPop '" << X << "' from stack" << endl;
				LLstack.pop_back();
				ip++;
			}
			else  //ƥ��ʧ�ܣ�������������
			{
				cout << "\tError: can't match '" << X << "' and '" << a << "'." << endl;
				return -1;
			}
		}
		else if (isUnTerm(tmpX))  // ����Ƿ��ս��
		{
			i = findIndex(tmpX);
			j = findIndexT(tmpa);
			if (table[i][j] == "NULL")  // �������Ӧ��Ϊ�գ�������������
			{
				cout << "\tError: can't find production when '" << X << "' meet '" << a << "'." << endl;
				return -1;
			}
			else
			{
				cout << "\tPop '" << X << "' from stack" << endl;
				cout << "\tMatch production: " << X << " -> " << table[i][j] << endl;
				LLstack.pop_back();
				if (table[i][j] != "@")  // �������ʽ���ǿմ�
				{
					cout << "\tPush '" << table[i][j] << "' reversed" << endl;
					for (int k = table[i][j].size() - 1; k >= 0; k--)
						LLstack.push_back(table[i][j][k]);
				}
			}
		}

		step++;

		// ��ȡջ�������봮��һ���ַ�
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