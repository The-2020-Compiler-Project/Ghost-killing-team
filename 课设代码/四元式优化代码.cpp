#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

struct Node						//DAG�ڵ�
{
	char Operation = '_';		//�������
	char MainSymbol = '_';		//����Ƿ�
	char Symbol[20];			//����Ƿ�
	Node* LeftChild;			//����
	Node* RightChild;			//�Һ���
};

int main()
{
	string Quaternion[50][4];	//��Ԫʽ����
	int p = 0;					//��Ԫʽ����ָ��
	ifstream ifile;
	ifile.open("�м����test.txt");
	while (1)					//������Ԫʽ
	{
		ifile >> Quaternion[p][0] >> Quaternion[p][1] >> Quaternion[p][2] >> Quaternion[p][3];
		p++;
		if (ifile.eof() != 0)break;						//�ļ�����ʱ����
	}
	for (int i = 0; i < p; i++)		//�����Ԫʽ����
	{
		cout << Quaternion[i][0] << "	" << Quaternion[i][1] << "	" << Quaternion[i][2] << "	" << Quaternion[i][3] << endl;
	}
	return 0;
}