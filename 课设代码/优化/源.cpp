#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

//��ĸ�뵥���ű�
char ch[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
			  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','\'' };

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

string ta[100][3];			//������ű�ʱ�������������
int addr2[100];				//���ű��еĵ���λΪ��ַ����int�Ϳ�����������
int t = 0;

//�����µ��ӽڵ㣬�����symbol���������λ�ϣ�����������ת��Ϊ���ַ������ֱ�Ƿ���
void createnode(int q,string symbol)
{
	dag[q].Symbol[0] = symbol;						//������Ƿ��������ַ�����Ϣ
	dag[q].s++;
	for (int i = 0; i < 53; i++)
	{
		if (symbol[0] == ch[i])break;			//������ַ�����break;
		else if (i == 52)						//������������������������������������Ϣ
		{
			dag[q].isnum = 1;
			stringstream ss;
			ss << symbol;
			ss >> dag[q].MainSymbolnum;
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
			if (a[0] == ch[i])				//������ַ����ǳ�tn������ĸ��ͷ���ַ���ʱ����Ȩ��Ϊ1
			{
				x = 1;
				break;
			}
			else if (i == 52)x = 2;			//������ַ���������ʱ����Ȩ��Ϊ2
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
	if (q == 0)
	{
		int comparenum;
		comparenum = compare(quaternion[d][1], quaternion[d][3]);
		if (comparenum < 2)										//����Ԫʽ��4λ���Ȩ�ش�
		{
			createnode(q, quaternion[d][3]);					//�����½�㲢ʹ���Ƿ�ͬʱ������Ԫʽ��2λ���4λ
			dag[q].Symbol[dag[q].s] = quaternion[d][1];
			dag[q].s += 1;
			q++;
		}
		else														//��4λȨ��С�ڵ��ڵ�2λ
		{
			createnode(q, quaternion[d][1]);
			dag[q].Symbol[dag[q].s] = quaternion[d][3];
			dag[q].s += 1;
			q++;
		}
	}
	else
	{
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
				if (comparenum < 2)										//����Ԫʽ��4λ���Ȩ�ش�
				{
					createnode(q, quaternion[d][3]);					//�����½�㲢ʹ���Ƿ�ͬʱ������Ԫʽ��2λ���4λ
					dag[q].Symbol[dag[q].s] = quaternion[d][1];
					dag[q].s += 1;
					q++;
					break;
				}
				else														//��4λȨ��С�ڵ��ڵ�2λ
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

//�����нڵ���Ѱ���Ƿ��е�dλ��Ԫʽ��λ��Ϊposition��Ԫ�أ���û���򴴽�һ���������ظýڵ�ָ��
node* findelement(int d,int position)
{
	node* ex = NULL;
	for (int i = q - 1; i >= 0; i--)								//�����нڵ��дӺ���ǰѰ���Ƿ��д���
	{
		for (int j = 0; j < dag[i].s; j++)
		{
			if (quaternion[d][position] == dag[i].Symbol[j])		//���ҵ�������һ���ڵ�
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

//����Ԫʽ��1λΪ�������ʱ����
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

//���ݽṹ�����飨DAGͼ����������Ԫʽ
void coutquaternion()
{
	ofstream ofile;
	ofile.open("�м����_�Ż���.txt");
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

//��DAGͼ���Դ��ڵ�tn���浽���ű���
void savetable()
{
	ifstream ifile;										//������ű�
	ifile.open("���ű�_�Ż�ǰ.txt");
	while (1)
	{
		ifile >> ta[t][0] >> ta[t][1] >> ta[t][2] >> addr2[t];
		t++;
		if (ifile.eof() != 0)break;						//�ļ�����ʱ����
	}
	ifile.close();
	string da[100][4];									//�����Ż������Ԫʽ�����鿴�����������м����tn
	int dd = 0;
	ifile.open("�м����_�Ż���.txt");
	while (1)
	{
		ifile >> da[dd][0] >> da[dd][1] >> da[dd][2] >> da[dd][3];
		dd++;
		if (ifile.eof() != 0)break;
	}
	ifile.close();
	string hh[25];									//�����洢�Ѿ��������tn
	int h = 0;
	int havehad = 0;
	for (int i = 0; i < dd; i++)					//����Ԫʽ��Ѱ��tn
	{
		int a;
		a = value(da[i][3]);
		if (a == 0)
		{
			for (int k = 0; k < h; k++)			//���Ѵ����tn��Ѱ�ұȽϣ����Ƿ��ظ�
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
			hh[h] = da[i][3];					//��δ�ظ���������Ѵ�������
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
	ofile.open("���ű�_�Ż���.txt");
	for (int i = 0; i < t; i++)
		ofile << ta[i][0] << "	" << ta[i][1] << "	" << ta[i][2] << "	" << addr2[i] << endl;
	ofile.close();
}

int main()
{
	ifstream ifile;				//���ļ�
	ifile.open("�м����_�Ż�ǰ.txt");
	while (1)					//������Ԫʽ
	{
		ifile >> quaternion[p][0] >> quaternion[p][1] >> quaternion[p][2] >> quaternion[p][3];
		p++;
		if (ifile.eof() != 0)break;						//�ļ�����ʱ����
	}
	ifile.close();
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
	coutquaternion();
	savetable();
	return 0;
}