#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

//字母表
char ch[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
			  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };

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

//创建新的子节点，将标记symbol放在主标记位上，若是数字则转换为数字放在数字标记符上
void createnode(node a,string symbol)
{
	a.Symbol[0] = symbol;						//用主标记符来保存字符串信息
	a.s++;
	for (int i = 0; i < 52; i++)
	{
		if (symbol[0] == ch[i])break;			//如果是字符串则break
		else if (i == 51)						//如果是数字则用数字主标记来保存其数字信息
		{
			a.isnum = 1;
			stringstream ss;
			ss << symbol;
			ss >> a.MainSymbolnum;
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
			string c;
			stringstream ss;
			ss << i;
			ss >> c;
			if (a[1] == 'c')
			{
				x = 0;
				break;
			}
		}
	}
	if (x == -1)
	{
		for (int i = 0; i < 52; i++)
		{
			if (a[0] == ch[i])				//当这个字符串是除tn以外字母开头的字符串时，其权重为1
			{
				x = 1;
				break;
			}
			else if (i == 51)x = 2;			//当这个字符串是数字时，其权重为2
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
			if (comparenum == 1)										//若四元式第4位标记权重大
			{
				createnode(dag[q], quaternion[d][3]);					//创建新结点并使其标记符同时含有四元式第2位与第4位
				dag[q].Symbol[dag[q].s] = quaternion[d][1];
				dag[q].s += 2;
				q++;
			}
			else														//第4位权重小于等于第2位
			{
				createnode(dag[q], quaternion[d][1]);
				dag[q].Symbol[dag[q].s] = quaternion[d][3];
				dag[q].s += 2;
				q++;
			}
		}
	}
	if (havehad == 1)
	{
		dag[addr].Symbol[dag[addr].s] = quaternion[d][or1_3];
		dag[addr].s++;
	}
}

//当四元式第1位为运算符号时读入
void read_calculate(int d)
{
	for (int i = 0; i < q; i++)
	{
		for (int j = 0; j < dag[i].s; j++)
		{
			if (quaternion[d][1] == dag[i].Symbol[j])
			{

			}
		}
	}
}

//根据结构体数组（DAG图）来生成四元式
void createquaternion(node a)
{

}

int main()
{
	ifstream ifile;				//打开文件
	ifile.open("中间代码test.txt");
	while (1)					//读入四元式
	{
		ifile >> quaternion[p][0] >> quaternion[p][1] >> quaternion[p][2] >> quaternion[p][3];
		p++;
		if (ifile.eof() != 0)break;						//文件结束时跳出
	}
	/*
	for (int i = 0; i < p; i++)							//输出四元式测试
	{
		cout << quaternion[i][0] << "	" << quaternion[i][1] << "	" << quaternion[i][2] << "	" << quaternion[i][3] << endl;
	}
	*/
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
	return 0;
}