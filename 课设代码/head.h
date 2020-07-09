#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

//**************�������******************
struct token {  //token�ṹ��
	int code;  //token������ţ�1�ؼ��֣�2�����3��ʶ����4�ַ���5����
	int value; //token��ֵ
};
struct symbol {  //���ű�ṹ
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
void FUNC();			//��������
void PARA();			//�βζ���
void EXTRA_PARA();		//�����β�
void STATEMENT_LIST();	//����б�
void STATEMENT_LIST1();	//����б�'
void STATEMENT();		//���
void DEF_STATEMENT();	//�������
void EXTRA_ID();		//�����ʶ��
void ASSI_STATEMENT();	//��ֵ���
void ASSI_CONTENT();	//��ֵ����
void EXPRESSION();		//���ʽ
void EXPRESSION1();		//���ʽ'
void T();				//���ʽ�������ս��T
void T1();				//���ʽ�������ս��T1
void F();				//���ʽ�������ս��F
//void ID();			//��ʶ��
//void CONSTANT();		//����
void PUSH(string str);	//������ջ
string POP();
string NEWT();
void ASSI();			//���ɸ�ֵ��Ԫʽ
void GEQ(string str);	//���ɱ��ʽ��Ԫʽ

void yuyifenxi();		//���������������


//**************�Ż�******************
struct node									//DAG�ڵ�
{
	int isnum = 0;							//�Ƿ�Ϊ����
	string operation = "_";					//�������
	float MainSymbolnum = -1;				//���ֱ�Ƿ�
	string Symbol[10];						//��Ƿ���Simbol[0]Ϊ����Ƿ�
	int s = 0;								//��Ƿ�����
	node* LeftChild = NULL;					//����
	node* RightChild = NULL;				//�Һ���
};
void createnode(int q, string symbol);		//�����µ��ӽڵ㣬�����symbol���������λ�ϣ�����������ת��Ϊ���ַ������ֱ�Ƿ���
int value(string a);						//����Ȩ��ֵ�����֣�2���ַ�����1���м����tn��0
int compare(string a, string b);			//�Ƚϱ�����ȼ���a<b������ 1��a=b������ 2��a>b������ 3
void read_equal(int d);						//����Ԫʽ��1λΪ��=��ʱ����
node* findelement(int d, int position);		//�����нڵ���Ѱ���Ƿ��е�dλ��Ԫʽ��λ��Ϊposition��Ԫ�أ���û���򴴽�һ���������ظýڵ�ָ��
void read_calculate(int d);					//����Ԫʽ��1λΪ�������ʱ����
void coutquaternion();						//���ݽṹ�����飨DAGͼ����������Ԫʽ
void savetable();							//��DAGͼ���Դ��ڵ�tn���浽���ű���

void youhua();								//�Ż���������
