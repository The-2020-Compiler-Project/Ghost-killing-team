#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

//字母与单引号表
char ch[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
			  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','\'' };

//DAG节点
struct node
{
	int isnum = 0;				//是否为数字
	string operation = "_";		//运算符号
	float MainSymbolnum = -1;	//数字标记符
	string Symbol[10];			//标记符，Simbol[0]为主标记符
	int s = 0;					//标记符数量
	node* LeftChild = NULL;		//左孩子
	node* RightChild = NULL;	//右孩子
};

string quaternion[100][4];	//四元式数组
int p = 0;					//四元式数组指针
node dag[100];				//DAG图节点数组
int q = 0;					//DAG图节点数组指针

string ta[100][3];			//读入符号表时将其填入此数组
int addr2[100];				//符号表中的第四位为地址，用int型可以易于增减
int t = 0;

//创建新的子节点，将标记symbol放在主标记位上，若是数字则转换为数字放在数字标记符上
void createnode(int q,string symbol)
{
	dag[q].Symbol[0] = symbol;						//用主标记符来保存字符串信息
	dag[q].s++;
	for (int i = 0; i < 53; i++)
	{
		if (symbol[0] == ch[i])break;			//如果是字符串则break;
		else if (i == 52)						//如果是数字则用数字主标记来保存其数字信息
		{
			dag[q].isnum = 1;
			stringstream ss;
			ss << symbol;
			ss >> dag[q].MainSymbolnum;
		}
	}
}

//计算权重值，数字：2，字符串：1，中间变量tn：0
int value(string a)
{
	int x = -1;
	if (a[0] == 't')						//当这个字符串是tn时（n为任意数字），其权重为0
	{
		for (int i = 1; i <= 9; i++)
		{
			char c;
			stringstream ss;
			ss << i;
			ss >> c;
			if (a[1] == c)
			{
				x = 0;
				break;
			}
		}
	}
	if (x == -1)
	{
		for (int i = 0; i < 53; i++)
		{
			if (a[0] == ch[i])				//当这个字符串是除tn以外字母开头的字符串时，其权重为1
			{
				x = 1;
				break;
			}
			else if (i == 52)x = 2;			//当这个字符串是数字时，其权重为2
		}
	}
	return x;
}

//比较标记优先级，a<b：返回 1；a=b：返回 2；a>b：返回 3
int compare(string a, string b)
{
	int x, y;								//x为a的权重，y为b的权重
	x = value(a);
	y = value(b);
	if (x < y)
		return 1;
	else if (x = y)
		return 2;
	else return 3;
}

//当四元式第1位为“=”时读入
void read_equal(int d)
{
	int havehad = 0;							//是否找到一个存在的标记
	int addr;									//所在位置
	int or1_3;									//找到相等标识符时记录四元式中另一个字符串的位数
	if (q == 0)
	{
		int comparenum;
		comparenum = compare(quaternion[d][1], quaternion[d][3]);
		if (comparenum < 2)										//若四元式第4位标记权重大
		{
			createnode(q, quaternion[d][3]);					//创建新结点并使其标记符同时含有四元式第2位与第4位
			dag[q].Symbol[dag[q].s] = quaternion[d][1];
			dag[q].s += 1;
			q++;
		}
		else														//第4位权重小于等于第2位
		{
			createnode(q, quaternion[d][1]);
			dag[q].Symbol[dag[q].s] = quaternion[d][3];
			dag[q].s += 1;
			q++;
		}
	}
	else
	{
		for (int i = 0; i < q; i++)					//在DAG节点中搜索
		{
			for (int j = 0; j < dag[i].s; j++)		//在一个节点中的所有标记符中搜索
			{
				if (quaternion[d][1] == dag[i].Symbol[j])				//若找到了一个标记符与四元式第2位相等，
				{														//则将该标记符的节点序号记录下来，并将havehad置为1
					havehad = 1;
					addr = i;
					or1_3 = 3;
					break;
				}
				if (quaternion[d][3] == dag[i].Symbol[j])				//找到了一个标记符与四元式第4位相等
				{
					havehad = 1;
					addr = i;
					or1_3 = 1;
					break;
				}
			}
			if (havehad == 1)											//找到了便跳出
				break;
			else if (i == q - 1)										//如果目前不存在这个标记
			{
				int comparenum;
				comparenum = compare(quaternion[d][1], quaternion[d][3]);
				if (comparenum < 2)										//若四元式第4位标记权重大
				{
					createnode(q, quaternion[d][3]);					//创建新结点并使其标记符同时含有四元式第2位与第4位
					dag[q].Symbol[dag[q].s] = quaternion[d][1];
					dag[q].s += 1;
					q++;
					break;
				}
				else														//第4位权重小于等于第2位
				{
					createnode(q, quaternion[d][1]);
					dag[q].Symbol[dag[q].s] = quaternion[d][3];
					dag[q].s += 1;
					q++;
					break;
				}
			}
		}
	}
	if (havehad == 1)
	{
		int com;
		com = compare(dag[addr].Symbol[0], quaternion[d][or1_3]);
		if (com >= 2)
		{
			dag[addr].Symbol[dag[addr].s] = quaternion[d][or1_3];
			dag[addr].s++;
		}
		else
		{
			dag[addr].Symbol[dag[addr].s] = dag[addr].Symbol[0];
			dag[addr].Symbol[0] = quaternion[d][or1_3];
			dag[addr].s++;
		}
	}
}

//在所有节点中寻找是否有第d位四元式中位置为position的元素，若没有则创建一个，并返回该节点指针
node* findelement(int d,int position)
{
	node* ex = NULL;
	for (int i = q - 1; i >= 0; i--)								//在所有节点中从后往前寻找是否有存在
	{
		for (int j = 0; j < dag[i].s; j++)
		{
			if (quaternion[d][position] == dag[i].Symbol[j])		//若找到了这样一个节点
			{
				ex = &dag[i];
				return ex;
			}
		}
	}
	createnode(q, quaternion[d][position]);
	q++;
	ex = &dag[q - 1];
	return ex;
}

//当四元式第1位为运算符号时读入
void read_calculate(int d)
{
	node* left = NULL, * right = NULL;
	left = findelement(d, 1);
	right = findelement(d, 2);
	createnode(q, quaternion[d][3]);
	q++;
	dag[q - 1].LeftChild = left;
	dag[q - 1].RightChild = right;
	dag[q - 1].operation = quaternion[d][0];
}

//根据结构体数组（DAG图）来生成四元式
void coutquaternion()
{
	ofstream ofile;
	ofile.open("中间代码_优化后.txt");
	for (int i = 0; i < q; i++)
	{
		if ((dag[i].operation == "_") && (dag[i].s > 1))
		{
			for (int j = 1; j < dag[i].s; j++)
				ofile << "=" << "	" << dag[i].Symbol[0] << "	" << "_" << "	" << dag[i].Symbol[j] << endl;
		}
		if (dag[i].operation != "_")
		{
			if (((*dag[i].LeftChild).isnum == 1) && ((*dag[i].RightChild).isnum == 1))
			{
				dag[i].isnum = 1;
				if (dag[i].operation == "+")
					dag[i].MainSymbolnum = (*dag[i].LeftChild).MainSymbolnum + (*dag[i].RightChild).MainSymbolnum;
				else if (dag[i].operation == "-")
					dag[i].MainSymbolnum = (*dag[i].LeftChild).MainSymbolnum - (*dag[i].RightChild).MainSymbolnum;
				else if (dag[i].operation == "*")
					dag[i].MainSymbolnum = (*dag[i].LeftChild).MainSymbolnum * (*dag[i].RightChild).MainSymbolnum;
				else if (dag[i].operation == "/")
					dag[i].MainSymbolnum = (*dag[i].LeftChild).MainSymbolnum / (*dag[i].RightChild).MainSymbolnum;
				dag[i].Symbol[dag[i].s] = dag[i].Symbol[0];
				dag[i].s++;
				stringstream ss;
				ss << dag[i].MainSymbolnum;
				ss >> dag[i].Symbol[0];
				int v = 1;
				v = value(dag[i].Symbol[dag[i].s - 1]);
				if(v)
					ofile << "=" << "	" << dag[i].Symbol[0] << "	" << "_" << "	" << dag[i].Symbol[dag[i].s - 1] << endl;
			}
			else
			{
				ofile << dag[i].operation << "	" << (*dag[i].LeftChild).Symbol[0] << "	" << (*dag[i].RightChild).Symbol[0] << "	" << dag[i].Symbol[0] << endl;
				for (int j = 1; j < dag[i].s; j++)
				{
					int va;
					va = value(dag[i].Symbol[j]);
					if (va == 1)
						ofile << "=" << "	" << dag[i].Symbol[0] << "	" << "_" << "	" << dag[i].Symbol[j] << endl;
				}
			}
		}
	}
	ofile.close();
}

//将DAG图中仍存在的tn保存到符号表中
void savetable()
{
	ifstream ifile;										//读入符号表
	ifile.open("符号表_优化前.txt");
	while (1)
	{
		ifile >> ta[t][0] >> ta[t][1] >> ta[t][2] >> addr2[t];
		t++;
		if (ifile.eof() != 0)break;						//文件结束时跳出
	}
	ifile.close();
	string da[100][4];									//读入优化后的四元式用来查看保留下来的中间变量tn
	int dd = 0;
	ifile.open("中间代码_优化后.txt");
	while (1)
	{
		ifile >> da[dd][0] >> da[dd][1] >> da[dd][2] >> da[dd][3];
		dd++;
		if (ifile.eof() != 0)break;
	}
	ifile.close();
	string hh[25];									//用来存储已经加入过的tn
	int h = 0;
	int havehad = 0;
	for (int i = 0; i < dd; i++)					//在四元式中寻找tn
	{
		int a;
		a = value(da[i][3]);
		if (a == 0)
		{
			for (int k = 0; k < h; k++)			//在已存过的tn中寻找比较，看是否重复
			{
				if (da[i][3] == hh[k])
				{
					havehad = 1;
					break;
				}
			}
			if (havehad == 1)
			{
				havehad = 0;
				continue;
			}
			hh[h] = da[i][3];					//若未重复则将其存在已存数组中
			h++;
			if ((ta[t - 1][1] == "1") || (ta[t - 1][1] == "3"))
				addr2[t] = addr2[t - 1] + 4;
			else if (ta[t - 1][1] == "2")
				addr2[t] = addr2[t - 1] + 1;
			string type1, type2, type;
			int num1 = 1, num2 = 1;
			num1 = value(da[i][1]);
			num2 = value(da[i][2]);
			if (num1 == 2)
			{
				int size1;
				size1 = da[i][1].size();
				type1 = "1";
				for (int j = 0; j < size1; j++)
				{
					if (da[i][1][j] == '.')
					{
						type1 = "3";
						break;
					}
				}
			}
			if (num2 == 2)
			{
				int size2;
				type2 = "1";
				size2 = da[i][2].size();
				for (int j = 0; j < size2; j++)
				{
					if (da[i][2][j] == '.')
					{
						type1 = "3";
						break;
					}
				}
			}
			for (int m = 0; m < t; m++)
			{
				if ((da[i][1] == ta[m][0]) && (num1 == 1))
				{
					type1 = ta[m][1];
				}
				if ((da[i][2] == ta[m][0]) && (num2 == 1))
				{
					type2 = ta[m][1];
				}
			}
			if ((type1 == "1") && (type2 == "1"))
				type = "1";
			else type = "3";
			ta[t][0] = da[i][3];
			ta[t][1] = type;
			ta[t][2] = "1";
			t++;
		}
	}
	if ((ta[t - 1][1] == "1") || (ta[t - 1][1] == "3"))
		addr2[0] = addr2[t - 1] + 4;
	else if (ta[t - 1][1] == "2")
		addr2[0] = addr2[t - 1] + 1;
	ofstream ofile;
	ofile.open("符号表_优化后.txt");
	for (int i = 0; i < t; i++)
		ofile << ta[i][0] << "	" << ta[i][1] << "	" << ta[i][2] << "	" << addr2[i] << endl;
	ofile.close();
}

int main()
{
	ifstream ifile;				//打开文件
	ifile.open("中间代码_优化前.txt");
	while (1)					//读入四元式
	{
		ifile >> quaternion[p][0] >> quaternion[p][1] >> quaternion[p][2] >> quaternion[p][3];
		p++;
		if (ifile.eof() != 0)break;						//文件结束时跳出
	}
	ifile.close();
	for (int d = 0; d < p; d++)							//顺序读入四元式数组
	{
		if (quaternion[d][0] == "=")					//当操作符为“=”时
		{
			read_equal(d);
			continue;
		}
		if (quaternion[d][0] == "+" || quaternion[d][0] == "-" || quaternion[d][0] == "*" || quaternion[d][0] == "/")
		{
			read_calculate(d);
		}
	}
	coutquaternion();
	savetable();
	return 0;
}