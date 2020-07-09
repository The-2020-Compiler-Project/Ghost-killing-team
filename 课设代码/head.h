#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

//**************语义分析******************
struct token {  //token结构体
	int code;  //token的类别编号，1关键字，2界符，3标识符，4字符，5常数
	int value; //token的值
};
struct symbol {  //符号表结构
	string name;
	int type;
	int cat;
	int addr;
};
struct four {
	string op;
	string arg1;
	string arg2;
	string result;
};
bool Isdata_type(string word);
string token_to_T(token t);
string token_to_T(token t);
int state_tran(int state, int num);
void token_to_SYNBL(int tline, int sline);
void FUNC();			//主函数体
void PARA();			//形参定义
void EXTRA_PARA();		//额外形参
void STATEMENT_LIST();	//语句列表
void STATEMENT_LIST1();	//语句列表'
void STATEMENT();		//语句
void DEF_STATEMENT();	//定义语句
void EXTRA_ID();		//额外标识符
void ASSI_STATEMENT();	//赋值语句
void ASSI_CONTENT();	//赋值内容
void EXPRESSION();		//表达式
void EXPRESSION1();		//表达式'
void T();				//表达式衍生非终结符T
void T1();				//表达式衍生非终结符T1
void F();				//表达式衍生非终结符F
//void ID();			//标识符
//void CONSTANT();		//常数
void PUSH(string str);	//入语义栈
string POP();
string NEWT();
void ASSI();			//生成赋值四元式
void GEQ(string str);	//生成表达式四元式

void yuyifenxi();		//语义分析函数声明


//**************优化******************
struct node									//DAG节点
{
	int isnum = 0;							//是否为数字
	string operation = "_";					//运算符号
	float MainSymbolnum = -1;				//数字标记符
	string Symbol[10];						//标记符，Simbol[0]为主标记符
	int s = 0;								//标记符数量
	node* LeftChild = NULL;					//左孩子
	node* RightChild = NULL;				//右孩子
};
void createnode(int q, string symbol);		//创建新的子节点，将标记symbol放在主标记位上，若是数字则转换为数字放在数字标记符上
int value(string a);						//计算权重值，数字：2，字符串：1，中间变量tn：0
int compare(string a, string b);			//比较标记优先级，a<b：返回 1；a=b：返回 2；a>b：返回 3
void read_equal(int d);						//当四元式第1位为“=”时读入
node* findelement(int d, int position);		//在所有节点中寻找是否有第d位四元式中位置为position的元素，若没有则创建一个，并返回该节点指针
void read_calculate(int d);					//当四元式第1位为运算符号时读入
void coutquaternion();						//根据结构体数组（DAG图）来生成四元式
void savetable();							//将DAG图中仍存在的tn保存到符号表中

void youhua();								//优化函数声明
