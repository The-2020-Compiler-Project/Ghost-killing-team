#include "head.h"
using namespace std;

struct tok {  //tok�ṹ��
	int code;  //tok������ţ�1�ؼ��֣�2�����3��ʶ����4�ַ���5����
	int value; //tok��ֵ
	int line;	//tok���ڴ����е�����
};
struct symb {  //���ű�ṹ
	string name;
	int type;
	int cat;
	int addr;
};

symb SYNBL[1000];		//�������ű�
int SYNBLnum = 0;
tok tok_list[1000];  //����tok����
int tok_list_num = 0;
int tok_line = 1;		//��ʼ������������Ϊ1
string KTN[50];
int KTNnum = 0;
string PTN[50];
int PTNnum = 0;
string ITN[1000];
int ITNnum = 0;
string CHA[1000];
int CHAnum = 0;
string CTN[1000];
int CTNnum = 0;

//�����֣��ؼ���
string KTNkeep[44] = { "auto", "const", "for", "register", "try", "virtual", "bool", "else", "friend", "namespace",
"switch", "struct", "typedef", "void", "break", "continue", "enum", "goto", "new", "return",
"case", "default", "if", "short", "while", "delete", "extern", "private", "this", "union",
"char", "do", "false", "int", "protected", "sizeof", "class", "double", "float", "long",
"public", "static", "true", "using" };
//�����ַ�
char OtherP[10] = { '{', '}', '(', ')', ',', ';', ':', '?', '[',']'};
//�����ַ�
char USELESS[4] = { ' ','\t' };
//�жϹؼ���
bool IsKTN(string word) {
	for (int i = 0; i < 44; i++) {
		if (KTNkeep[i] == word) return true;
	}
	return false;
}
//�ж������ַ�
bool IsOtherP(char ch) {
	for (int i = 0; i < 10; i++) {
		if (OtherP[i] == ch) return true;
	}
	return false;
}
//�ж϶����ַ�
bool IsUSELESS(char ch) {
	for (int i = 0; i < 2; i++) {
		if (USELESS[i] == ch)return true;
	}
	return false;
}
//״̬ת�ƺ���
int state_tran(int state, char ch) {
	switch (state) {
	case 1: {
		if (ch == '\r' || ch == '\n') {
			tok_line++;
			return 1;
		}
		else if (IsUSELESS(ch))return 1;
		else if (isalpha(ch) || ch == '_')return 2;
		else if (isdigit(ch))return 3;
		else if (ch == '\'')return 6;
		else if (ch == '\"')return 9;
		else if (ch == '*' || ch == '!' || ch == '=')return 12;
		else if (ch == '+')return 14;
		else if (ch == '-')return 17;
		else if (ch == '<')return 20;
		else if (ch == '>')return 23;
		else if (ch == '&')return 26;
		else if (ch == '|')return 28;
		else if (IsOtherP(ch))return 30;
		else if (ch == '/')return 31;
		else return -1;
	}
	case 2: {
		if (isalpha(ch) || isdigit(ch) || ch == '_')return 2;
		else return 0;
	}
	case 3: {
		if (isdigit(ch))return 3;
		else if (ch == '.')return 4;
		else return 0;
	}
	case 4: {
		if (isdigit(ch))return 5;
		else return -1;
	}
	case 5: {
		if (isdigit(ch))return 5;
		else return 0;
	}
	case 6:return 7; 
	case 7: {
		if (ch == '\'')return 8;
		else return -1; 
	}
	case 8:return 0;
	case 9:return 10;
	case 10: {
		if (ch != '\"')return 10;
		else return 11;
	}
	case 11:return 0;
	case 12: {
		if (ch == '=')return 13;
		else return 0;
	}
	case 13:return 0;
	case 14: {
		if (ch == '=')return 15;
		else if (ch == '+')return 16;
		else return 0;
	}
	case 15:return 0;
	case 16:return 0;
	case 17: {
		if (ch == '=')return 18;
		else if (ch == '-')return 19;
		else return 0;
	}
	case 18:return 0;
	case 19:return 0;
	case 20: {
		if (ch == '=')return 21;
		else if (ch == '<')return 22;
		else return 0;
	}
	case 21:return 0;
	case 22:return 0;
	case 23: {
		if (ch == '=')return 24;
		else if (ch == '>')return 25;
		else return 0;
	}
	case 24:return 0;
	case 25:return 0;
	case 26: {
		if (ch == '&')return 27;
		else return 0;
	}
	case 27:return 0;
	case 28: {
		if (ch == '|')return 29;
		else return 0;
	}
	case 29:return 0;
	case 30:return 0;
	case 31: {
		if (ch == '=')return 32;
		else if (ch == '/')return 33;
		else if (ch == '*')return 34;
		else return 0;
	}
	case 32:return 0;
	case 33: {
		if (ch != '\n')return 33;
		else return 0;
	}
	case 34: {
		if (ch != '*')return 34;
		else return 35;
	}
	case 35: {
		if (ch != '/')return 34;
		else return 36;
	}
	case 36:return 0;
	default:return -1;
	}
}

//tok���ж�
int fenlei(int state_b, string tok) {
	switch (state_b) {
	case 2: {
		if (IsKTN(tok))return 1;
		else return 3;
	}
	case 3:
	case 5:return 5;
	case 8:return 4;
	case 11:return 5;
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:return 2;
	case 33:
	case 36:return 7;
	default:return -1;
	}
}

int search(string str, string T[], int num) {//���ұ�������Ԫ�أ��������
	int i = 0;
	while (i < num) {
		if (str == T[i]) {
			return i;
		}
		i++;
	}
	return i;
}

//����tok�͸�����
void output(int type,string str) {
	switch (type) {
	case 1: {			//���뵽�ؼ��ֱ�
		int i = search(str, KTN, KTNnum);
		tok_list[tok_list_num].code = 1;
		tok_list[tok_list_num].value = i+1;
		tok_list[tok_list_num].line = tok_line;
		tok_list_num++;
		if (i == KTNnum) {
			KTN[KTNnum] = str;
			KTNnum++;
		}
		break;
	}
	case 2: {			//���뵽�����
		int i = search(str, PTN, PTNnum);
		tok_list[tok_list_num].code = 2;
		tok_list[tok_list_num].value = i + 1;
		tok_list[tok_list_num].line = tok_line;
		tok_list_num++;
		if (i == PTNnum) {
			PTN[PTNnum] = str;
			PTNnum++;
		}
		break;
	}
	case 3: {			//���뵽��ʶ����
		int i = search(str, ITN, ITNnum);
		tok_list[tok_list_num].code = 3;
		tok_list[tok_list_num].value = i + 1;
		tok_list[tok_list_num].line = tok_line;
		tok_list_num++;
		if (i == ITNnum) {			//���뵽���ű�
			ITN[ITNnum] = str;
			SYNBL[SYNBLnum].name = str;
			SYNBL[SYNBLnum].type = 0;
			SYNBL[SYNBLnum].cat = 0;
			SYNBL[SYNBLnum].addr = 0;
			SYNBLnum++;
			ITNnum++;
		}
		break;
	}
	case 4: {			//���뵽�ַ���
		int i = search(str, CHA, CHAnum);
		tok_list[tok_list_num].code = 4;
		tok_list[tok_list_num].value = i + 1;
		tok_list[tok_list_num].line = tok_line;
		tok_list_num++;
		if (i == CHAnum) {
			CHA[CHAnum] = str;
			CHAnum++;
		}
		break;
	}
	case 5: {			//���뵽������
		int i = search(str, CTN, CTNnum);
		tok_list[tok_list_num].code = 5;
		tok_list[tok_list_num].value = i + 1;
		tok_list[tok_list_num].line = tok_line;
		tok_list_num++;
		if (i == CTNnum) {
			CTN[CTNnum] = str;
			CTNnum++;
		}
		break;
	}
	case 7:cout << "ע�� ��" << endl << str << endl; break; //ʶ��ע��
	}
}

void cifafenxi() {
	char ch;
	int state = 1, state_b;
	string tok;
	int type;
	ifstream in;    
	in.open("���Դ���.txt");   
	while ((ch = in.get()) != EOF) { 
		state_b = state;                   //�����Զ���״̬
		state = state_tran(state, ch);     //�Զ���״̬ת��
		if (state) {
			if (!(state == 1 && (IsUSELESS(ch) || ch == '\n' || ch == '\r'))) {
				tok = tok + ch;       //��дtok(״̬1ʱ����������ַ����룩
			}
		}
		else {
			type = fenlei(state_b, tok); //
			output(type,tok);
			tok = "";                    //���tok
			state = 1;                     //״̬��ʼ��
			in.seekg(-1, ios::cur);        //�ļ�ָ��ǰ��һλ
		}
	}
	in.close();

	/*for (int i = 0; i < KTNnnum; i++) {
		cout << KTN[i] << endl;
	}
	for (int i = 0; i < PTNNnum; i++) {
		cout << PTN[i] << endl;
	}
	for (int i = 0; i < ITNnum; i++) {
		cout << ITN[i] << endl;
	}
	for (int i = 0; i < CHAnum; i++) {
		cout << CHA[i] << endl;
	}
	for (int i = 0; i < CTNnum; i++) {
		cout << CTN[i] << endl;
	}
	for (int i = 0; i < SYNBLnum; i++) {
		cout << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
	}

	for (int i = 0; i < tok_list_num; i++) {
		cout << tok_list[i].code << "	" << tok_list[i].value << endl;
	}*/

	ofstream KTNfile;   //�򿪹ؼ��ֱ��ļ�
	KTNfile.open("����1�ؼ��ֱ�.txt");
	int i = 0;
	while (i < KTNnum) {//�ؼ��ֱ�������ļ�
		KTNfile << KTN[i] << endl;
		i++;
	}
	KTNfile.close();

	ofstream PTNfile;   //�򿪽�����ļ�
	PTNfile.open("����2�����.txt");
	i = 0;
	while (i < PTNnum) {//�����������ļ�
		PTNfile << PTN[i] << endl;
		i++;
	}
	PTNfile.close();

	ofstream ITNfile;   //�򿪱�ʶ�����ļ�
	ITNfile.open("����3��ʶ����.txt");
	i = 0;
	while (i < ITNnum) {//��ʶ����������ļ�
		ITNfile << ITN[i] << endl;
		i++;
	}
	ITNfile.close();

	ofstream CHAfile;   //���ַ����ļ�
	CHAfile.open("����4�ַ���.txt");
	i = 0;
	while (i < CHAnum) {//�ַ���������ļ�
		CHAfile << CHA[i] << endl;
		i++;
	}
	CHAfile.close();

	ofstream CTNfile;   //�򿪳������ļ�
	CTNfile.open("����5������.txt");
	i = 0;
	while (i < CTNnum) {//������������ļ�
		CTNfile << CTN[i] << endl;
		i++;
	}
	CTNfile.close();

	ofstream symbfile;   //�򿪷��ű��ļ�
	symbfile.open("���Է��ű��ʷ���.txt");
	i = 0;
	while (i < SYNBLnum) {//���ű�������ļ�
		symbfile << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
		i++;
	}
	symbfile.close();

	ofstream tokfile;   //��token�����ļ�
	tokfile.open("����tok.txt");
	i = 0;
	while (i < tok_list_num) {//token������ļ�
		tokfile << tok_list[i].code << "	" << tok_list[i].value << "	" << tok_list[i].line << endl;
		i++;
	}
	tokfile.close();
}