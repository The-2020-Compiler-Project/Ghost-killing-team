#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

struct Node						//DAG节点
{
	char Operation = '_';		//运算符号
	char MainSymbol = '_';		//主标记符
	char Symbol[20];			//副标记符
	Node* LeftChild;			//左孩子
	Node* RightChild;			//右孩子
};

int main()
{
	string Quaternion[50][4];	//四元式数组
	int p = 0;					//四元式数组指针
	ifstream ifile;
	ifile.open("中间代码test.txt");
	while (1)					//读入四元式
	{
		ifile >> Quaternion[p][0] >> Quaternion[p][1] >> Quaternion[p][2] >> Quaternion[p][3];
		p++;
		if (ifile.eof() != 0)break;						//文件结束时跳出
	}
	for (int i = 0; i < p; i++)		//输出四元式测试
	{
		cout << Quaternion[i][0] << "	" << Quaternion[i][1] << "	" << Quaternion[i][2] << "	" << Quaternion[i][3] << endl;
	}
	return 0;
}