#include "head.h"
using namespace std;

struct tok {  //tok结构体
	int code;  //tok的类别编号，1关键字，2界符，3标识符，4字符，5常数
	int value; //tok的值
	int line;	//tok所在代码中的行数
};
struct symb {  //符号表结构
	string name;
	int type;
	int cat;
	int addr;
};

symb SYNBL[1000];		//建立符号表
int SYNBLnum = 0;
tok tok_list[1000];  //建立tok序列
int tok_list_num = 0;
int tok_line = 1;		//初始代码所在行数为1
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

//保留字，关键字
string KTNkeep[44] = { "auto", "const", "for", "register", "try", "virtual", "bool", "else", "friend", "namespace",
"switch", "struct", "typedef", "void", "break", "continue", "enum", "goto", "new", "return",
"case", "default", "if", "short", "while", "delete", "extern", "private", "this", "union",
"char", "do", "false", "int", "protected", "sizeof", "class", "double", "float", "long",
"public", "static", "true", "using" };
//其他字符
char OtherP[10] = { '{', '}', '(', ')', ',', ';', ':', '?', '[',']'};
//过滤字符
char USELESS[4] = { ' ','\t' };
//判断关键字
bool IsKTN(string word) {
	for (int i = 0; i < 44; i++) {
		if (KTNkeep[i] == word) return true;
	}
	return false;
}
//判断其他字符
bool IsOtherP(char ch) {
	for (int i = 0; i < 10; i++) {
		if (OtherP[i] == ch) return true;
	}
	return false;
}
//判断多余字符
bool IsUSELESS(char ch) {
	for (int i = 0; i < 2; i++) {
		if (USELESS[i] == ch)return true;
	}
	return false;
}
//状态转移函数
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

//tok类判断
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

int search(string str, string T[], int num) {//查找表中已有元素，返回序号
	int i = 0;
	while (i < num) {
		if (str == T[i]) {
			return i;
		}
		i++;
	}
	return i;
}

//输入tok和各个表
void output(int type,string str) {
	switch (type) {
	case 1: {			//输入到关键字表
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
	case 2: {			//输入到界符表
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
	case 3: {			//输入到标识符表
		int i = search(str, ITN, ITNnum);
		tok_list[tok_list_num].code = 3;
		tok_list[tok_list_num].value = i + 1;
		tok_list[tok_list_num].line = tok_line;
		tok_list_num++;
		if (i == ITNnum) {			//输入到符号表
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
	case 4: {			//输入到字符表
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
	case 5: {			//输入到常数表
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
	case 7:cout << "注释 ：" << endl << str << endl; break; //识别注释
	}
}

void cifafenxi() {
	char ch;
	int state = 1, state_b;
	string tok;
	int type;
	ifstream in;    
	in.open("测试代码.txt");   
	while ((ch = in.get()) != EOF) { 
		state_b = state;                   //储存自动机状态
		state = state_tran(state, ch);     //自动机状态转换
		if (state) {
			if (!(state == 1 && (IsUSELESS(ch) || ch == '\n' || ch == '\r'))) {
				tok = tok + ch;       //填写tok(状态1时不允许多余字符输入）
			}
		}
		else {
			type = fenlei(state_b, tok); //
			output(type,tok);
			tok = "";                    //清空tok
			state = 1;                     //状态初始化
			in.seekg(-1, ios::cur);        //文件指针前移一位
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

	ofstream KTNfile;   //打开关键字表文件
	KTNfile.open("测试1关键字表.txt");
	int i = 0;
	while (i < KTNnum) {//关键字表输出到文件
		KTNfile << KTN[i] << endl;
		i++;
	}
	KTNfile.close();

	ofstream PTNfile;   //打开界符表文件
	PTNfile.open("测试2界符表.txt");
	i = 0;
	while (i < PTNnum) {//界符表输出到文件
		PTNfile << PTN[i] << endl;
		i++;
	}
	PTNfile.close();

	ofstream ITNfile;   //打开标识符表文件
	ITNfile.open("测试3标识符表.txt");
	i = 0;
	while (i < ITNnum) {//标识符表输出到文件
		ITNfile << ITN[i] << endl;
		i++;
	}
	ITNfile.close();

	ofstream CHAfile;   //打开字符表文件
	CHAfile.open("测试4字符表.txt");
	i = 0;
	while (i < CHAnum) {//字符表输出到文件
		CHAfile << CHA[i] << endl;
		i++;
	}
	CHAfile.close();

	ofstream CTNfile;   //打开常数表文件
	CTNfile.open("测试5常数表.txt");
	i = 0;
	while (i < CTNnum) {//常数表输出到文件
		CTNfile << CTN[i] << endl;
		i++;
	}
	CTNfile.close();

	ofstream symbfile;   //打开符号表文件
	symbfile.open("测试符号表（词法）.txt");
	i = 0;
	while (i < SYNBLnum) {//符号表输出到文件
		symbfile << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
		i++;
	}
	symbfile.close();

	ofstream tokfile;   //打开token序列文件
	tokfile.open("测试tok.txt");
	i = 0;
	while (i < tok_list_num) {//token输出到文件
		tokfile << tok_list[i].code << "	" << tok_list[i].value << "	" << tok_list[i].line << endl;
		i++;
	}
	tokfile.close();
}