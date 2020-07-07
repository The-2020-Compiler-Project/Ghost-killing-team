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
string CH[1000];
string CT[1000];

string data_type[3] = { "int","float","char" };//�������������
bool Isdata_type(string word) {  //�ж��Ƿ�Ϊ��������
	for (int i = 0; i < 3; i++) {
		if (data_type[i] == word) return true;
	}
	return false;
}


/*string tokennum_to_T(int num){   //����token��ŷ���token����
	switch (token_list[num].code) {
	case 1:return KT[token_list[num].value];
	case 2:return PT[token_list[num].value];
	case 3:return IT[token_list[num].value];
	default:return 0;
	}
}*/

string token_to_T(token t) {  //����token���ض�Ӧ�ĵ���
	switch (t.code) {
	case 1:return KT[t.value - 1];
	case 2:return PT[t.value - 1];
	case 3:return IT[t.value - 1];
	case 4:return CH[t.value - 1];
	case 5:return CT[t.value - 1];
	default:return "error";
	}
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
		else {
			cout << "ERROR!" << endl;
			return -1;
		}
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
		else {
			cout << "ERROR!" << endl;
			return -1;
		}
	}
	case 3: {
		if (Isdata_type(token_to_T(token_list[num]))) {
			return 4;
		}
		else if (token_to_T(token_list[num]) == ")") {
			return 0;
		}
		else {
			cout << "ERROR!" << endl;
			return -1;
		}
	}
	case 4: {
		if (token_list[num].code == 3) {
			return 5;
		}
		else {
			cout << "ERROR!" << endl;
			return -1;
		}
	}
	case 5: {
		if (token_to_T(token_list[num]) == ")") {
			return 0;
		}
		else if (token_to_T(token_list[num]) == ",") {
			return 3;
		}
		else {
			cout << "ERROR!" << endl;
			return -1;
		}
	}
	default:return -1;
	}
}

void token_to_SYNBL(int tline,int sline) {  //tokenɨ��������д���ű�
	token last_data_type;  //��ʱ����ؼ�token��Ϣ
	string last_symbol_name;   //��¼��һ����ȡ�ı�ʶ��
	int offset = 0;		//��¼���
	int state = 0;		//״̬
	int last_state = 0; //ǰһ��״̬
	int lookup[6][6] = {1,0,2,3,0,6,
						4,0,2,0,0,0,
						0,5,0,0,0,0,
						0,0,1,0,0,6,
						0,0,0,4,0,0,
						0,0,0,0,5,0 }; //lookup[��ǰ״̬][ǰһ״̬]{�������}
	int i = 0;  //token����
	while (i < tline) {   //��һ��ȡtoken
		last_state = state;
		state = state_tran(state,i);  //״̬ת��
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
		case 6: {
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
					SYNBL[j].cat = 3;//3���β�����
				}
			}
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

struct four {
	string op;
	string arg1;
	string arg2;
	string result;
};

four four_list[100];
int four_list_num = 0;
string SEM[100];
int top = 0;

void FUNC();			//��������
void PARA();			//�βζ���
void EXTRA_PARA();		//�����β�
void STATEMENT_LIST();	//����б�
void STATEMENT_LIST1();	//����б�'
void STATEMENT();		//���
void DEF_STATEMENT();	//�������
void DATA_TYPE();		//��������
void EXTRA_ID();		//�����ʶ��
void ASSI_STATEMENT();	//��ֵ���
void ASSI_CONTENT();	//��ֵ����
void EXPRESSION();		//���ʽ
void EXPRESSION1();		//���ʽ'
void T();				//���ʽ�������ս��T
void T1();				//���ʽ�������ս��T1
void F();				//���ʽ�������ս��F
//void ID();				//��ʶ��
//void CONSTANT();		//����
void PUSH(string str);			//������ջ
void ASSI();			//���ɸ�ֵ��Ԫʽ
void GEQ(string str);				//���ɱ��ʽ��Ԫʽ

int token_num;
void FUNC() {			//��������
	//cout << "FUNC()" << endl;
	if (Isdata_type(token_to_T(token_list[token_num]))) {
		token_num++;
		if (token_list[token_num].code == 3) {
			token_num++;
			if (token_to_T(token_list[token_num]) == "(") {
				token_num++;
				PARA();
				if (token_to_T(token_list[token_num]) == ")") {
					token_num++;
					if (token_to_T(token_list[token_num]) == "{") {
						token_num++;
						STATEMENT_LIST();
						if (token_to_T(token_list[token_num]) == "}") {
							cout << "FINISH!" << endl;
						}
						else cout << "ERROR!" << endl;
					}
					else cout << "ERROR!" << endl;
				}
				else cout << "ERROR!" << endl;
			}
			else cout << "ERROR!" << endl;
		}
		else cout << "ERROR!" << endl;
	}
	else cout << "ERROR!" << endl;
}

void PARA() {			//�βζ���
	if (Isdata_type(token_to_T(token_list[token_num]))) {
		token_num++;
		if (token_list[token_num].code == 3) {
			token_num++;
			EXTRA_PARA();
		}
		else cout << "ERROR!" << endl;
	}
	else cout << "ERROR!" << endl;
}

void EXTRA_PARA() {		//�����β�
	if (token_to_T(token_list[token_num]) == ",") {
		token_num++;
		PARA();
	}
}

void STATEMENT_LIST() {	//����б�
	//cout << "STATEMENT_LIST()" << endl;
	STATEMENT();
	STATEMENT_LIST1();
}

void STATEMENT_LIST1(){//����б�'
	//cout << "STATEMENT_LIST1()" << endl;
	if (Isdata_type(token_to_T(token_list[token_num])) || token_list[token_num].code == 3) {
		STATEMENT_LIST();
	}
}

void STATEMENT() {		//���
	//cout << "STATEMENT()" << endl;
	if (Isdata_type(token_to_T(token_list[token_num]))) {
		DEF_STATEMENT();
		if (token_to_T(token_list[token_num]) == ";") {
			token_num++;
		}
		else cout << "ERROR!" << endl;
	}
	else if (token_list[token_num].code == 3) {
		ASSI_STATEMENT();
		if (token_to_T(token_list[token_num]) == ";") {
			token_num++;
		}
		else cout << "ERROR!" << endl;
	}
	else cout << "ERROR!" << endl;
}

void DEF_STATEMENT() {	//�������
	//cout << "DEF_STATEMENT()" << endl;
	if (Isdata_type(token_to_T(token_list[token_num]))) {
		token_num++;
		if (token_list[token_num].code == 3) {
			token_num++;
			EXTRA_ID();
		}
		else cout << "ERROR!" << endl;
	}
	else cout << "ERROR!" << endl;
}

/*void DATA_TYPE() {		//��������
	if (Isdata_type(token_to_T(token_list[token_num]))) {
		token_num++;
	}
	else cout << "ERROR!" << endl;
}*/

void EXTRA_ID() {		//�����ʶ��
	//cout << "EXTRA_ID()" << endl;
	if (token_to_T(token_list[token_num]) == ",") {
		token_num++;
		if (token_list[token_num].code == 3) {
			token_num++;
			EXTRA_ID();
		}
		else cout << "ERROR!" << endl;
	}
	/*else {
		token_num++;
	}*/
}

void ASSI_STATEMENT() {	//��ֵ���
	//cout << "ASSI_STATEMENT()" << endl;
	if (token_list[token_num].code == 3) {
		PUSH(token_to_T(token_list[token_num]));
		token_num++;
		if (token_to_T(token_list[token_num]) == "=") {
			token_num++;
			ASSI_CONTENT();
			ASSI();
		}
		else cout << "ERROR!" << endl;
	}
	else cout << "ERROR!" << endl;
}

void ASSI_CONTENT() {	//��ֵ����
	//cout << "ASSI_CONTENT()" << endl;
	if (token_list[token_num].code == 4) {
		PUSH(token_to_T(token_list[token_num]));
		token_num++;
	}
	else if (token_to_T(token_list[token_num]) == "(" || token_list[token_num].code == 3 || token_list[token_num].code == 5) {
		EXPRESSION();
	}
	else cout << "ERROR!" << endl;
}

void EXPRESSION() {		//���ʽ
	//cout << "EXPRESSION()" << endl;
	T();
	EXPRESSION1();
}

void EXPRESSION1() {	//���ʽ'
	//cout << "EXPRESSION1()" << endl;
	if (token_to_T(token_list[token_num]) == "+") {
		token_num++;
		T();
		GEQ("+");
		EXPRESSION1();
	}
	else if (token_to_T(token_list[token_num]) == "-") {
		token_num++;
		T();
		GEQ("-");
		EXPRESSION1();
	}
	/*else {
		token_num++;
	}*/
}

void T() {				//���ʽ�������ս��T
	//cout << "T()" << endl;
	F();
	T1();
}

void T1() {				//���ʽ�������ս��T1
	//cout << "T1()" << endl;
	if (token_to_T(token_list[token_num]) == "*") {
		token_num++;
		F();
		GEQ("*");
		T1();
	}
	else if (token_to_T(token_list[token_num]) == "/") {
		token_num++;
		F();
		GEQ("/");
		T1();
	}
	/*else {
		token_num++;
	}*/
}

void F() {				//���ʽ�������ս��F
	//cout << "F()" << endl;
	if (token_to_T(token_list[token_num]) == "(") {
		token_num++;
		EXPRESSION();
		if (token_to_T(token_list[token_num]) == ")") {
			token_num++;
		}
		else cout << "ERROR!" << endl;
	}
	else if (token_list[token_num].code == 3|| token_list[token_num].code == 5) {
		PUSH(token_to_T(token_list[token_num]));
		token_num++;
	}
	else cout << "ERROR!" << endl;
}

/*void ID() {				//��ʶ��
	if (token_list[token_num].code == 3) {
		token_num++;
	}
	else cout << "ERROR!" << endl;
}*/

/*void CONSTANT() {		//����
	if (token_list[token_num].code == 5) {
		token_num++;
	}
	else cout << "ERROR!" << endl;
}*/

void PUSH(string str) {			//������ջ
	cout << "PUSH(" << str << ")" << endl;
	SEM[top] = str;
	top++;
}
string POP() {
	top--;
	return SEM[top];
}
int T_num = 1;
string NEWT() {
	string t = "t" + to_string(T_num);
	T_num++;
	return t;
}
void ASSI() {			//���ɸ�ֵ��Ԫʽ
	cout << "ASSI()" << endl;
	four_list[four_list_num].op = "=";
	four_list[four_list_num].arg1 = POP();
	four_list[four_list_num].arg2 = "_";
	four_list[four_list_num].result = POP();
	four_list_num++;
}
void GEQ(string str) {				//���ɱ��ʽ��Ԫʽ
	cout << "GEQ(" << str << ")" << endl;
	string t = NEWT();
	four_list[four_list_num].op = str;
	four_list[four_list_num].arg2 = POP();
	four_list[four_list_num].arg1 = POP();
	four_list[four_list_num].result = t;
	PUSH(t);
	four_list_num++;
}

int main() {
	char out;
	ifstream symbolfile1;   //�򿪷��ű��ļ�
	symbolfile1.open("���Է��ű��ʷ���.txt");
	int SYNBL_line1 = 0;
	while (symbolfile1.peek() != EOF) {//���ļ��ж�����ű���Ϣ
		symbolfile1 >> SYNBL[SYNBL_line1].name >> SYNBL[SYNBL_line1].type >> SYNBL[SYNBL_line1].cat >> SYNBL[SYNBL_line1].addr;
		SYNBL_line1++;
		symbolfile1.get(out);
	}
	symbolfile1.close();
	/*cout << "��д���ű�ǰ:" << endl;
	for (int i = 0; i < SYNBL_line; i++) {
		cout << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
	}*/

	ifstream tokenfile;  //��token�����ļ�
	tokenfile.open("����token.txt");
	int token_line = 0;
	while (tokenfile.peek() != EOF) {  //���ļ��ж���token������Ϣ
		tokenfile >> token_list[token_line].code >> token_list[token_line].value;
		token_line++;
		tokenfile.get(out);
	}
	tokenfile.close();
	/*for (int i = 0; i < token_line; i++){
		cout << token_list[i].code << "	" << token_list[i].value << endl;
	}*/
	
	ifstream KTfile;
	KTfile.open("����1�ؼ��ֱ�.txt");
	int KT_line = 0;
	while (KTfile.peek() != EOF) {   //���ļ��ж���ؼ��ֱ�
		KTfile >> KT[KT_line];
		KT_line++;
		KTfile.get(out);
	}
	KTfile.close();
	/*for (int i = 0; i < KT_line; i++) {
		cout << KT[i] << endl;
	}*/
	
	ifstream PTfile;
	PTfile.open("����2�����.txt");
	int PT_line = 0;
	while (PTfile.peek() != EOF) {   //���ļ��ж�������
		PTfile >> PT[PT_line];
		PT_line++;
		PTfile.get(out);
	}
	PTfile.close();
	/*for (int i = 0; i < PT_line; i++) {
		cout << PT[i] << endl;
	}*/

	ifstream ITfile;
	ITfile.open("����3��ʶ����.txt");
	int IT_line = 0;
	while (ITfile.peek() != EOF) {   //���ļ��ж����ʶ����
		ITfile >> IT[IT_line];
		IT_line++;
		ITfile.get(out);
	}
	ITfile.close();
	/*for (int i = 0; i < IT_line; i++) {
		cout << IT[i] << endl;
	}*/

	ifstream CHfile;
	CHfile.open("����4�ַ���.txt");
	int CH_line = 0;
	while (CHfile.peek() != EOF) {   //���ļ��ж����ַ���
		CHfile >> CH[CH_line];
		CH_line++;
		CHfile.get(out);
	}
	CHfile.close();
	/*for (int i = 0; i < CH_line; i++) {
	cout << CH[i] << endl;
	}*/

	ifstream CTfile;
	CTfile.open("����5������.txt");
	int CT_line = 0;
	while (CTfile.peek() != EOF) {   //���ļ��ж��볣����
		CTfile >> CT[CT_line];
		CT_line++;
		CTfile.get(out);
	}
	CTfile.close();
	/*for (int i = 0; i < CT_line; i++) {
	cout << CT[i] << endl;
	}*/

	/*cout << "��д���ű�ǰ:" << endl;
	cout << "name" << "	" << "type" << "	" << "cat" << "	" << "addr" << endl;
	for (int i = 0; i < SYNBL_line; i++) {
		cout << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
	}
	*/
	token_to_SYNBL(token_line, SYNBL_line1);
	/*cout << "��д���ű��:" << endl;
	for (int i = 0; i < SYNBL_line; i++) {
		cout << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
	}
	cout << "type:1(int),2(char)��3(float)" << endl << "cat:1(����),2(����)" << endl;*/
	ofstream symbolfile2;   //�򿪷��ű��ļ�
	symbolfile2.open("���Է��ű����壩.txt");
	int i = 0;
	while (i< SYNBL_line1) {//���ļ��ж�����ű���Ϣ
		symbolfile2 << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
		i++;
	}
	symbolfile2.close();
	FUNC();
	/*for (int i = 0; i < four_list_num; i++) {
		cout << four_list[i].op << "	" << four_list[i].arg1 << "	" << four_list[i].arg2 << "	" << four_list[i].result << endl;
	}*/
	ofstream four_file;   //�򿪷��ű��ļ�
	four_file.open("�����м����.txt");
	i = 0;
	while (i < four_list_num) {//���ļ��ж�����ű���Ϣ
		four_file << four_list[i].op << "	" << four_list[i].arg1 << "	" << four_list[i].arg2 << "	" << four_list[i].result << endl;
		i++;
	}
	four_file.close();

	system("pause");
	return 0;
}