#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<stack>

#define STATE_NUM 16		// ״̬��
#define TERM_NUM 8			// �ս����������action����
#define UNTERM_NUM 3		// ���ս����������goto����
#define G_NUM 9				// ����ʽ��
#define file_path "C:/Users/87290/Desktop/PROGRAM/����ԭ��-�ʷ�����/�ҵĴ���/lexical_analysis/lexical.txt"

using namespace std;

// ����Ǻ�����ÿ�����Լ������Ϣ
typedef struct words
{
	char sign;				// �Ǻ� 
	string attribute;		// ����
}WORDS;

// ���������ı���
typedef struct item
{
	string action;			// action����(��ԼR/����S/����ACC)��goto���(G)
	int state;				// ����
}ITEM;


// �����ս��
vector<string> TERM;			
// ������ս��
vector<string> UNTERM;			
// ������ʽ
string LEFT[] = { "E'", "E", "E", "E", "T", "T", "T", "F", "F"};   
// �Ҳ�����ʽ
string RIGHT[] = { "E", "E+T", "E-T", "T", "T*F", "T/F", "F", "(E)", "d" };
// �Ҳ�����ʽ����
int LEN[] = { 1, 3, 3, 1, 3, 3, 1, 3, 1 };
// ������
ITEM table[STATE_NUM][TERM_NUM + UNTERM_NUM];
// ������(���Դʷ�����)
vector<WORDS> input;

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

// ���ջ������
void outPutStack(vector<string> stack)
{
	cout << "\tsymbol: ";
	for (int i = 0; i < stack.size(); i++)
		cout << stack[i] << " ";
	cout << endl;
}
void outPutStack(vector<int> stack)
{
	cout << "\tstate: ";
	for (int i = 0; i < stack.size(); i++)
		cout << stack[i] << " ";
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

// �㷨4.3����LR����
int LRanalyse()
{
	vector<int> State;		// ״̬ջ
	vector<string> Symbol;	// ����ջ
	int step = 1;			// ��¼����������
	int ip = 0;				// ��¼���봮������λ��
	int i, j;
	int S = 0;				// ջ��״̬
	char a;					// ip��ָ����
	string tmpa;			// a��string��ʽ�ĸ������������Ͽ⺯��

	State.push_back(0);
	Symbol.push_back("E'");

	a = input[ip].sign;
	tmpa.clear();
	tmpa.push_back(a);

	while (1)
	{
		cout << "Step " << step << ": " << endl;

		if (isTerm(tmpa))
			j = findIndexT(tmpa);
		else
		{
			cout << "ERROR: can't find '" << a << "' in Termin Set" << endl;
			return -1;
		}

		if (table[S][j].action == "S")  // �ƽ�����
		{
			outPutStack(State);
			outPutStack(Symbol);
			outPutIn(ip);
			State.push_back(table[S][j].state);
			Symbol.push_back(tmpa);
			ip++;
			cout << "\tShift " << table[S][j].state << " into state stack" << endl;
		}
		else if (table[S][j].action == "R")  // ��Լ����
		{
			outPutStack(State);
			outPutStack(Symbol);
			outPutIn(ip);
			for (int k = 0; k < LEN[table[S][j].state]; k++)
			{
				State.pop_back();
				Symbol.pop_back();
			}
			Symbol.push_back(LEFT[table[S][j].state]);
			cout << "\tReduce by " << LEFT[table[S][j].state] << " -> " << RIGHT[table[S][j].state] << endl;
			if (isUnTerm(LEFT[table[S][j].state]))
				j = findIndex(LEFT[table[S][j].state]) + TERM_NUM;
			else
			{
				cout << "ERROR: can't find '" << a << "' in UnTermin Set" << endl;
				return -1;
			}
			S = State.back();
			State.push_back(table[S][j].state);
		}
		else if (table[S][j].action == "ACC")
		{
			cout << "\tAnalyse success" << endl;
			break;
		}
		else
		{
			cout << "\titem at table[" << S << "," << TERM[j] << "] is " << table[S][j].action << endl;
			cout << "\tAnalyse fail" << endl;
			return -1;
		}

		step++;

		S = State.back();
		if (ip < input.size())
		{
			a = input[ip].sign;
			tmpa.clear();
			tmpa.push_back(a);
		}
	}

	return 0;
}

// ��ʼ��
void initProgram()
{
	// ���ս��
	string F[] = { "E", "T", "F" };
	// �ս��
	string T[] = { "+", "-", "*", "/", "(", ")", "d", "$" };

	// ��ʼ���ս�������ս��
	for (int i = 0; i < TERM_NUM; i++)
	{
		TERM.push_back(T[i]);
	}
	for (int i = 0; i < UNTERM_NUM; i++)
	{
		UNTERM.push_back(F[i]);
	}

	// ��ʼ��������
	// ��ȫ����ʼ��Ϊ��(NULL, -1)
	for (int i = 0; i < STATE_NUM; i++)
		for (int j = 0; j < TERM_NUM + UNTERM_NUM; j++)
			table[i][j].action = "NULL", table[i][j].state = -1;

	table[0][4] = { "S", 4 };
	table[0][6] = { "S", 5 };
	table[0][8] = { "G", 1 };
	table[0][9] = { "G", 2 };
	table[0][10] = { "G", 3 };

	table[1][0] = { "S", 6 };
	table[1][1] = { "S", 7 };
	table[1][7] = { "ACC", -1 };
					    
	table[2][0] = { "R", 3 };
	table[2][1] = { "R", 3 };
	table[2][2] = { "S", 8 };
	table[2][3] = { "S", 9 };
	table[2][5] = { "R", 3 };
	table[2][7] = { "R", 3 };
					    
	table[3][0] = { "R", 6 };
	table[3][1] = { "R", 6 };
	table[3][2] = { "R", 6 };
	table[3][3] = { "R", 6 };
	table[3][5] = { "R", 6 };
	table[3][7] = { "R", 6 };
					    
	table[4][4] = { "S", 4 };
	table[4][6] = { "S", 5 };
	table[4][8] = { "G", 10 };
	table[4][9] = { "G", 2 };
	table[4][10] = { "G", 3 };

	table[5][0] = { "R", 8 };
	table[5][1] = { "R", 8 };
	table[5][2] = { "R", 8 };
	table[5][3] = { "R", 8 };
	table[5][5] = { "R", 8 };
	table[5][7] = { "R", 8 };

	table[6][4] = { "S", 4 };
	table[6][6] = { "S", 5 };
	table[6][9] = { "G", 11 };
	table[6][10] = { "G", 3 };

	table[7][4] = { "S", 4 };
	table[7][6] = { "S", 5 };
	table[7][9] = { "G", 12 };
	table[7][10] = { "G", 13 };

	table[8][4] = { "S", 4 };
	table[8][6] = { "S", 5 };
	table[8][10] = { "G", 13 };

	table[9][4] = { "S", 4 };
	table[9][6] = { "S", 5 };
	table[9][10] = { "G", 14 };

	table[10][0] = { "S", 6 };
	table[10][1] = { "S", 7 };
	table[10][5] = { "S", 15 };

	table[11][0] = { "R", 1 };
	table[11][1] = { "R", 1 };
	table[11][2] = { "S", 8 };
	table[11][3] = { "S", 9 };
	table[11][5] = { "R", 1 };
	table[11][7] = { "R", 1 };

	table[12][0] = { "R", 2 };
	table[12][1] = { "R", 2 };
	table[12][2] = { "S", 8 };
	table[12][3] = { "S", 9 };
	table[12][5] = { "R", 2 };
	table[12][7] = { "R", 2 };

	table[13][0] = { "R", 4 };
	table[13][1] = { "R", 4 };
	table[13][2] = { "R", 4 };
	table[13][3] = { "R", 4 };
	table[13][5] = { "R", 4 };
	table[13][7] = { "R", 4 };
					  
	table[14][0] = { "R", 5 };
	table[14][1] = { "R", 5 };
	table[14][2] = { "R", 5 };
	table[14][3] = { "R", 5 };
	table[14][5] = { "R", 5 };
	table[14][7] = { "R", 5 };
					  
	table[15][0] = { "R", 7 };
	table[15][1] = { "R", 7 };
	table[15][2] = { "R", 7 };
	table[15][3] = { "R", 7 };
	table[15][5] = { "R", 7 };
	table[15][7] = { "R", 7 };

	// ���(������)
	cout << "table:" << endl;
	for (int i = 0; i < STATE_NUM; i++)
	{
		for (int j = 0; j < TERM_NUM + UNTERM_NUM; j++)
			if (table[i][j].state != -1)
				cout << table[i][j].action << table[i][j].state << "\t";
			else
				cout << table[i][j].action << "\t";
		cout << endl;
	}
		
}


int main()
{
	initProgram();

	cout << "input stream: " << endl;
	getInput();
	cout << endl;

	LRanalyse();

	return 0;
}