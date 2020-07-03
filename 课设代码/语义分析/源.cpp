#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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

symbol SYNBL[1000];		//�������ű�
token token_list[1000];  //����token����
string KT[50];
string PT[50];
string IT[1000];

bool Isdata_type(string word) {  //�ж��Ƿ�Ϊ��������
	for (int i = 0; i < 3; i++) {
		if (data_type[i] == word) return true;
	}
	return false;
}

int state_tran(int state, int num) {  //ɨ��token��д���ű��״̬ת������
	switch (state) {
	case 0: {
		if (Isdata_type(token_to_T(token_list[num]))) {  //��״̬0��⵽�������͵ĵ��ʺ�ת��Ϊ״̬1
			return 1;
		}
		else return 0;
	}
	case 1: {
		if (token_list[num].code == 3) {  //״̬1 �����ʶ�� ״̬2
			return 2;
		}
		else return 0;
	}
	case 2: {
		if (token_to_T(token_list[num]) == ",") {//���붺�ţ�����״̬1��׼����ͬ���͵���һ����ʶ��
			return 1;
		}
		else if (token_to_T(token_list[num]) == ";") {//����ֺţ��ص�״̬0
			return 0;
		}
		else if (token_to_T(token_list[num]) == "(") {//���������ţ��ж�Ϊ������ʶ����ת��״̬3
			return 3;
		}
		else return 0;
	}
	case 3:return 0;  //Ϊ����������������״̬
	case 4:return 0;
	case 5:return 0;
	default:return 0;
	}
}

void token_sacnning(int tline, int sline) {  //tokenɨ��������д���ű�
	token last_data_type;  //��ʱ����ؼ�token��Ϣ
	string last_symbol_name;   //��¼��һ����ȡ�ı�ʶ��
	int offset = 0;		//��¼���
	int state = 0;		//״̬
	int last_state = 0; //ǰһ��״̬
	int lookup[6][6] = { 1,0,2,3,0,0,
						4,0,2,0,0,0,
						0,5,0,0,0,0,
						0,0,1,0,0,0,
						0,0,0,0,0,0,
						0,0,0,0,0,0 }; //lookup[��ǰ״̬][ǰһ״̬]{�������}
	int i = 0;  //token����
	while (i < tline) {   //��һ��ȡtoken
		last_state = state;
		state = state_tran(state, i);  //״̬ת��
		switch (lookup[state][last_state]) {
		case 0: {
			cout << "err!" << endl;
			break;
		}
		case 1: {  //�޲���
			break;
		}
		case 2: {  //�������
			for (int j = 0; j < sline; j++) {
				if (last_symbol_name == SYNBL[j].name) {
					if (token_to_T(last_data_type) == "int") {
						SYNBL[j].type = 1;
						SYNBL[j].addr = offset;
						offset += 4;
					}
					else if (token_to_T(last_data_type) == "char") {
						SYNBL[j].type = 2;
						SYNBL[j].addr = offset;
						offset += 1;
					}
					else if (token_to_T(last_data_type) == "float") {
						SYNBL[j].type = 3;
						SYNBL[j].addr = offset;
						offset += 4;
					}
					SYNBL[j].cat = 1;//1�Ǳ�������
				}
			}
			break;
		}
		case 3: {  //�������
			for (int j = 0; j < sline; j++) {
				if (last_symbol_name == SYNBL[j].name) {
					if (token_to_T(last_data_type) == "int") {
						SYNBL[j].type = 1;
					}
					else if (token_to_T(last_data_type) == "char") {
						SYNBL[j].type = 2;
					}
					else if (token_to_T(last_data_type) == "float") {
						SYNBL[j].type = 3;
					}
					SYNBL[j].cat = 2;//1�Ǻ�������
				}
			}
			break;
		}
		case 4: {  //��¼��������
			last_data_type = token_list[i];
			break;
		}
		case 5: {  //��¼��ʶ����
			last_symbol_name = token_to_T(token_list[i]);
			break;
		}
		default:break;
		}
		i++;
	}
	for (int j = 0; j < sline; j++) {  //�����д����ռ�������
		if (2 == SYNBL[j].cat) {
			SYNBL[j].addr = offset;
			break;
		}
	}
}

int main() {
	ifstream symbolfile;   //�򿪷��ű��ļ�
	symbolfile.open("���Է��ű��ʷ���.txt");
	int SYNBL_line = 0;
	while (symbolfile.eof() == 0) {//���ļ��ж�����ű���Ϣ
		symbolfile >> SYNBL[SYNBL_line].name >> SYNBL[SYNBL_line].type >> SYNBL[SYNBL_line].cat >> SYNBL[SYNBL_line].addr;
		SYNBL_line++;
	}
	symbolfile.close();

	ifstream tokenfile;  //��token�����ļ�
	tokenfile.open("����token.txt");
	int token_line = 0;
	while (tokenfile.eof() == 0) {  //���ļ��ж���token������Ϣ
		tokenfile >> token_list[token_line].code >> token_list[token_line].value;
		token_line++;
	}
	tokenfile.close();

	ifstream KTfile;
	KTfile.open("����1�ؼ��ֱ�.txt");
	int KT_line = 0;
	while (KTfile.eof() == 0) {   //���ļ��ж���ؼ��ֱ�
		KTfile >> KT[KT_line];
		KT_line++;
	}
	KTfile.close();

	ifstream PTfile;
	PTfile.open("����2�����.txt");
	int PT_line = 0;
	while (PTfile.eof() == 0) {   //���ļ��ж�������
		PTfile >> PT[PT_line];
		PT_line++;
	}
	PTfile.close();

	ifstream ITfile;
	ITfile.open("����3��ʶ����.txt");
	int IT_line = 0;
	while (ITfile.eof() == 0) {   //���ļ��ж����ʶ����
		ITfile >> IT[IT_line];
		IT_line++;
	}
	ITfile.close();

	system("pause");
	return 0;
}