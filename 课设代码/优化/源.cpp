#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

//��ĸ��
char ch[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
			  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };

//DAG�ڵ�
struct node
{
	int isnum = 0;				//�Ƿ�Ϊ����
	string operation = "_";		//�������
	float MainSymbolnum = -1;	//���ֱ�Ƿ�
	string Symbol[10];			//��Ƿ���Simbol[0]Ϊ����Ƿ�
	int s = 0;					//��Ƿ�����
	node* LeftChild = NULL;		//����
	node* RightChild = NULL;	//�Һ���
};

string quaternion[100][4];	//��Ԫʽ����
int p = 0;					//��Ԫʽ����ָ��
node dag[100];				//DAGͼ�ڵ�����
int q = 0;					//DAGͼ�ڵ�����ָ��

//�����µ��ӽڵ㣬�����symbol���������λ�ϣ�����������ת��Ϊ���ַ������ֱ�Ƿ���
void createnode(node a,string symbol)
{
	a.Symbol[0] = symbol;						//������Ƿ��������ַ�����Ϣ
	a.s++;
	for (int i = 0; i < 52; i++)
	{
		if (symbol[0] == ch[i])break;			//������ַ�����break
		else if (i == 51)						//������������������������������������Ϣ
		{
			a.isnum = 1;
			stringstream ss;
			ss << symbol;
			ss >> a.MainSymbolnum;
		}
	}
}

//����Ȩ��ֵ�����֣�2���ַ�����1���м����tn��0
int value(string a)
{
	int x = -1;
	if (a[0] == 't')						//������ַ�����tnʱ��nΪ�������֣�����Ȩ��Ϊ0
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
			if (a[0] == ch[i])				//������ַ����ǳ�tn������ĸ��ͷ���ַ���ʱ����Ȩ��Ϊ1
			{
				x = 1;
				break;
			}
			else if (i == 51)x = 2;			//������ַ���������ʱ����Ȩ��Ϊ2
		}
	}
	return x;
}

//�Ƚϱ�����ȼ���a<b������ 1��a=b������ 2��a>b������ 3
int compare(string a, string b)
{
	int x, y;								//xΪa��Ȩ�أ�yΪb��Ȩ��
	x = value(a);
	y = value(b);
	if (x < y)
		return 1;
	else if (x = y)
		return 2;
	else return 3;
}

//����Ԫʽ��1λΪ��=��ʱ����
void read_equal(int d)
{
	int havehad = 0;							//�Ƿ��ҵ�һ�����ڵı��
	int addr;									//����λ��
	int or1_3;									//�ҵ���ȱ�ʶ��ʱ��¼��Ԫʽ����һ���ַ�����λ��
	for (int i = 0; i < q; i++)					//��DAG�ڵ�������
	{
		for (int j = 0; j < dag[i].s; j++)		//��һ���ڵ��е����б�Ƿ�������
		{
			if (quaternion[d][1] == dag[i].Symbol[j])				//���ҵ���һ����Ƿ�����Ԫʽ��2λ��ȣ�
			{														//�򽫸ñ�Ƿ��Ľڵ���ż�¼����������havehad��Ϊ1
				havehad = 1;
				addr = i;
				or1_3 = 3;
				break;
			}
			if (quaternion[d][3] == dag[i].Symbol[j])				//�ҵ���һ����Ƿ�����Ԫʽ��4λ���
			{
				havehad = 1;
				addr = i;
				or1_3 = 1;
				break;
			}
		}
		if (havehad == 1)											//�ҵ��˱�����
			break;
		else if (i == q - 1)										//���Ŀǰ������������
		{
			int comparenum;
			comparenum = compare(quaternion[d][1], quaternion[d][3]);
			if (comparenum == 1)										//����Ԫʽ��4λ���Ȩ�ش�
			{
				createnode(dag[q], quaternion[d][3]);					//�����½�㲢ʹ���Ƿ�ͬʱ������Ԫʽ��2λ���4λ
				dag[q].Symbol[dag[q].s] = quaternion[d][1];
				dag[q].s += 2;
				q++;
			}
			else														//��4λȨ��С�ڵ��ڵ�2λ
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

//����Ԫʽ��1λΪ�������ʱ����
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

//���ݽṹ�����飨DAGͼ����������Ԫʽ
void createquaternion(node a)
{

}

int main()
{
	ifstream ifile;				//���ļ�
	ifile.open("�м����test.txt");
	while (1)					//������Ԫʽ
	{
		ifile >> quaternion[p][0] >> quaternion[p][1] >> quaternion[p][2] >> quaternion[p][3];
		p++;
		if (ifile.eof() != 0)break;						//�ļ�����ʱ����
	}
	/*
	for (int i = 0; i < p; i++)							//�����Ԫʽ����
	{
		cout << quaternion[i][0] << "	" << quaternion[i][1] << "	" << quaternion[i][2] << "	" << quaternion[i][3] << endl;
	}
	*/
	for (int d = 0; d < p; d++)							//˳�������Ԫʽ����
	{
		if (quaternion[d][0] == "=")					//��������Ϊ��=��ʱ
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