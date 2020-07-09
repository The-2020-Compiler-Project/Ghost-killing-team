#include "head.h"
using namespace std;

struct token {  //token结构体
	int code;  //token的类别编号，1关键字，2界符，3标识符，4字符，5常数
	int value; //token的值
	int line; //token的行数
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

symbol SYNBL[1000];		//建立符号表
int SYNBL_line1 = 0;
token token_list[1000];  //建立token序列
string KT[50];
string PT[50];
string IT[1000];
string CH[1000];
string CT[1000];
string data_type[3] = { "int","float","char" };//定义的数据类型

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
void PUSH(string str);	//入语义栈
string POP();
string NEWT();
void ASSI();			//生成赋值四元式
void GEQ(string str);	//生成表达式四元式

bool Isdata_type(string word) {  //判断是否为数据类型
	for (int i = 0; i < 3; i++) {
		if (data_type[i] == word) return true;
	}
	return false;
}
void COUTERROR(int num) {	//输出错误信息
	cout << "ERROR: \"" << token_to_T(token_list[num]) << "\"		行数: " << token_list[num].line << endl; 
}

/*string tokennum_to_T(int num){   //根据token序号返回token内容
	switch (token_list[num].code) {
	case 1:return KT[token_list[num].value];
	case 2:return PT[token_list[num].value];
	case 3:return IT[token_list[num].value];
	default:return 0;
	}
}*/
string token_to_T(token t) {  //根据token返回对应的单词
	switch (t.code) {
	case 1:return KT[t.value - 1];
	case 2:return PT[t.value - 1];
	case 3:return IT[t.value - 1];
	case 4:return CH[t.value - 1];
	case 5:return CT[t.value - 1];
	default:return "ERROR!";
	}
}
int state_tran(int state, int num) {  //扫描token填写符号表的状态转换函数
	switch (state) {
	case 0: {
		if (Isdata_type(token_to_T(token_list[num]))) {  //从状态0检测到数据类型的单词后转换为状态1
			return 1;
		}
		else return 0;
	}
	case 1: {
		if (token_list[num].code == 3) {  //状态1 读入标识符 状态2
			return 2;
		}
		else {
			COUTERROR(num);
			return -1;
		}
	}
	case 2: {  
		if (token_to_T(token_list[num]) == ",") {//读入逗号，返回状态1，准备读同类型的下一个标识符
			return 1;
		}
		else if (token_to_T(token_list[num]) == ";") {//读入分号，回到状态0
			return 0;
		}
		else if (token_to_T(token_list[num]) == "(") {//读入左括号，判断为函数标识符，转到状态3
			return 3;
		}
		else {
			COUTERROR(num);
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
			COUTERROR(num);
			return -1;
		}
	}
	case 4: {
		if (token_list[num].code == 3) {
			return 5;
		}
		else {
			COUTERROR(num);
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
			COUTERROR(num);
			return -1;
		}
	}
	default:return -1;
	}
}
void token_to_SYNBL(int tline,int sline) {  //token扫描器，填写符号表
	token last_data_type;  //临时储存关键token信息
	string last_symbol_name;   //记录上一个读取的标识符
	int offset = 0;		//记录域宽
	int state = 0;		//状态
	int last_state = 0; //前一个状态
	int lookup[6][6] = {1,0,2,1,0,6,
						4,0,2,0,0,0,
						0,5,0,0,0,0,
						0,0,3,0,0,6,
						0,0,0,4,0,0,
						0,0,0,0,5,0 }; //lookup[当前状态][前一状态]{操作编号}
	int i = 0;  //token行数
	while (i < tline) {   //逐一读取token
		last_state = state;
		state = state_tran(state, i);  //状态转换
		switch (lookup[state][last_state]) {
		case 0: {
			cout << "ERROR！" << endl;
			break;
		}
		case 1: {  //无操作
			break;
		}
		case 2: {  //变量填表
			for (int j = 0; j < sline; j++) {
				if (last_symbol_name == SYNBL[j].name) {
					if (SYNBL[j].type != 0) {
						cout << "ERROR:" << SYNBL[j].name << "重定义" << endl;
					}
					else {
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
						SYNBL[j].cat = 1;//1是变量种类
					}
				}
			}
			break;
		}
		case 3: {  //函数填表
			for (int j = 0; j < sline; j++) {
				if (last_symbol_name == SYNBL[j].name) {
					if (SYNBL[j].type != 0) {
						cout << "ERROR:" << SYNBL[j].name << "重定义" << endl;
					}
					else {
						if (token_to_T(last_data_type) == "int") {
							SYNBL[j].type = 1;
						}
						else if (token_to_T(last_data_type) == "char") {
							SYNBL[j].type = 2;
						}
						else if (token_to_T(last_data_type) == "float") {
							SYNBL[j].type = 3;
						}
						SYNBL[j].cat = 2;//1是函数种类
					}
				}
			}
			break;
		}
		case 4: {  //记录数据类型
			last_data_type = token_list[i];
			break;
		}
		case 5: {  //记录标识符名
			last_symbol_name = token_to_T(token_list[i]);
			break;
		}
		case 6: {//形参填表
			for (int j = 0; j < sline; j++) {
				if (last_symbol_name == SYNBL[j].name) {
					if (SYNBL[j].type != 0) {
						cout << "ERROR:" << SYNBL[j].name << "重定义" << endl;
					}
					else {
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
						SYNBL[j].cat = 3;//3是形参种类
					}
				}
			}
			break;
		}
		default:break;
		}
		i++;
	}
	for (int j = 0; j < sline; j++) {  //最后填写函数占用总域宽
		if (2 == SYNBL[j].cat) {
			SYNBL[j].addr = offset;
			break;
		}
	}
}

four four_list[100];
int four_list_num = 0;

string SEM[100];
int top = 0;

int token_num;

bool CHECK_DEF(string str) {
	for (int i = 0; i < SYNBL_line1; i++) {
		if (str == SYNBL[i].name) {
			if (SYNBL[i].type == 0) {
				return true;
			}
			else return false;
		}
	}
}

void FUNC() {			//主函数体
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
							//cout << "FINISH!" << endl;
						}
						else cout << "ERROR: 缺少\"}\""<<endl;
					}
					else COUTERROR(token_num);
				}
				else COUTERROR(token_num);
			}
			else COUTERROR(token_num);
		}
		else COUTERROR(token_num);
	}
	else COUTERROR(token_num);
}

void PARA() {			//形参定义
	if (Isdata_type(token_to_T(token_list[token_num]))) {
		token_num++;
		if (token_list[token_num].code == 3) {
			token_num++;
			EXTRA_PARA();
		}
		else COUTERROR(token_num);
	}
	else if (token_to_T(token_list[token_num]) == ")") {
		return;
	}
	else COUTERROR(token_num);
}

void EXTRA_PARA() {		//额外形参
	if (token_to_T(token_list[token_num]) == ",") {
		token_num++;
		if (Isdata_type(token_to_T(token_list[token_num]))) {
			token_num++;
			if (token_list[token_num].code == 3) {
				token_num++;
				EXTRA_PARA();
			}
			else COUTERROR(token_num);
		}
		else COUTERROR(token_num);
	}
	else if (token_to_T(token_list[token_num]) == ")") {
		return;
	}
	else COUTERROR(token_num);
}

void STATEMENT_LIST() {	//语句列表
	//cout << "STATEMENT_LIST()" << endl;
	STATEMENT();
	STATEMENT_LIST1();
}

void STATEMENT_LIST1(){//语句列表'
	//cout << "STATEMENT_LIST1()" << endl;
	if (Isdata_type(token_to_T(token_list[token_num])) || token_list[token_num].code == 3) {
		STATEMENT_LIST();
	}
	else if (token_to_T(token_list[token_num]) == "}") {
		return;
	}
	else COUTERROR(token_num);
}

void STATEMENT() {		//语句
	//cout << "STATEMENT()" << endl;
	if (Isdata_type(token_to_T(token_list[token_num]))) {
		DEF_STATEMENT();
		if (token_to_T(token_list[token_num]) == ";") {
			token_num++;
		}
		else COUTERROR(token_num);
	}
	else if (token_list[token_num].code == 3) {
		ASSI_STATEMENT();
		if (token_to_T(token_list[token_num]) == ";") {
			token_num++;
		}
		else COUTERROR(token_num);
	}
	else COUTERROR(token_num);
}

void DEF_STATEMENT() {	//定义语句
	//cout << "DEF_STATEMENT()" << endl;
	if (Isdata_type(token_to_T(token_list[token_num]))) {
		token_num++;
		if (token_list[token_num].code == 3) {
			token_num++;
			EXTRA_ID();
		}
		else COUTERROR(token_num);
	}
	else COUTERROR(token_num);
}

void EXTRA_ID() {		//额外标识符
	//cout << "EXTRA_ID()" << endl;
	if (token_to_T(token_list[token_num]) == ",") {
		token_num++;
		if (token_list[token_num].code == 3) {
			token_num++;
			EXTRA_ID();
		}
		else COUTERROR(token_num);
	}
	else if (token_to_T(token_list[token_num]) == ";") {
		return;
	}
	else COUTERROR(token_num);
}

void ASSI_STATEMENT() {	//赋值语句
	//cout << "ASSI_STATEMENT()" << endl;
	if (token_list[token_num].code == 3) {
		if (CHECK_DEF(token_to_T(token_list[token_num]))) {
			cout << "ERROR: " << token_to_T(token_list[token_num]) << "未定义！" << endl;
		}
		PUSH(token_to_T(token_list[token_num]));
		token_num++;
		if (token_to_T(token_list[token_num]) == "=") {
			token_num++;
			ASSI_CONTENT();
			ASSI();
		}
		else COUTERROR(token_num);
	}
	else COUTERROR(token_num);
}

void ASSI_CONTENT() {	//赋值内容
	//cout << "ASSI_CONTENT()" << endl;
	if (token_list[token_num].code == 4) {
		PUSH(token_to_T(token_list[token_num]));
		token_num++;
	}
	else if (token_to_T(token_list[token_num]) == "(" || token_list[token_num].code == 3 || token_list[token_num].code == 5) {
		EXPRESSION();
	}
	else COUTERROR(token_num);
}

void EXPRESSION() {		//表达式
	//cout << "EXPRESSION()" << endl;
	T();
	EXPRESSION1();
}

void EXPRESSION1() {	//表达式'
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
	else if (token_to_T(token_list[token_num]) == ";"|| token_to_T(token_list[token_num]) == ")") {
		return;
	}
	else COUTERROR(token_num);
}

void T() {				//表达式衍生非终结符T
	//cout << "T()" << endl;
	F();
	T1();
}

void T1() {				//表达式衍生非终结符T1
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
	else if (token_to_T(token_list[token_num]) == "+" || token_to_T(token_list[token_num]) == "-" || token_to_T(token_list[token_num]) == ")" || token_to_T(token_list[token_num]) == ";") {
		return;
	}
	else COUTERROR(token_num);
}

void F() {				//表达式衍生非终结符F
	//cout << "F()" << endl;
	if (token_to_T(token_list[token_num]) == "(") {
		token_num++;
		EXPRESSION();
		if (token_to_T(token_list[token_num]) == ")") {
			token_num++;
		}
		else COUTERROR(token_num);
	}
	else if (token_list[token_num].code == 3 ) {
		if (CHECK_DEF(token_to_T(token_list[token_num]))) {
			cout << "ERROR: " << token_to_T(token_list[token_num]) << "未定义！" << endl;
		}
		PUSH(token_to_T(token_list[token_num]));
		token_num++;
	}
	else if (token_list[token_num].code == 5) {
		PUSH(token_to_T(token_list[token_num]));
		token_num++;
	}
	else COUTERROR(token_num);
}

void PUSH(string str) {			//入语义栈
	//cout << "PUSH(" << str << ")" << endl;

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
void ASSI() {			//生成赋值四元式
	//cout << "ASSI()" << endl;
	four_list[four_list_num].op = "=";
	four_list[four_list_num].arg1 = POP();
	four_list[four_list_num].arg2 = "_";
	four_list[four_list_num].result = POP();
	four_list_num++;
}
void GEQ(string str) {				//生成表达式四元式
	//cout << "GEQ(" << str << ")" << endl;
	string t = NEWT();
	four_list[four_list_num].op = str;
	four_list[four_list_num].arg2 = POP();
	four_list[four_list_num].arg1 = POP();
	four_list[four_list_num].result = t;
	PUSH(t);
	four_list_num++;
}

void yuyifenxi() {
	char out;
	ifstream symbolfile1;   //打开符号表文件
	symbolfile1.open("测试符号表（词法）.txt");
	while (symbolfile1.peek() != EOF) {//从文件中读入符号表信息
		symbolfile1 >> SYNBL[SYNBL_line1].name >> SYNBL[SYNBL_line1].type >> SYNBL[SYNBL_line1].cat >> SYNBL[SYNBL_line1].addr;
		SYNBL_line1++;
		symbolfile1.get(out);
	}
	symbolfile1.close();
	/*cout << "填写符号表前:" << endl;
	for (int i = 0; i < SYNBL_line; i++) {
		cout << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
	}*/

	ifstream tokenfile;  //打开token序列文件
	tokenfile.open("测试tok.txt");
	int token_line = 0;
	while (tokenfile.peek() != EOF) {  //从文件中读入token序列信息
		tokenfile >> token_list[token_line].code >> token_list[token_line].value >> token_list[token_line].line;
		token_line++;
		tokenfile.get(out);
	}
	tokenfile.close();
	/*for (int i = 0; i < token_line; i++){
		cout << token_list[i].code << "	" << token_list[i].value << endl;
	}*/
	
	ifstream KTfile;
	KTfile.open("测试1关键字表.txt");
	int KT_line = 0;
	while (KTfile.peek() != EOF) {   //从文件中读入关键字表
		KTfile >> KT[KT_line];
		KT_line++;
		KTfile.get(out);
	}
	KTfile.close();
	/*for (int i = 0; i < KT_line; i++) {
		cout << KT[i] << endl;
	}*/
	
	ifstream PTfile;
	PTfile.open("测试2界符表.txt");
	int PT_line = 0;
	while (PTfile.peek() != EOF) {   //从文件中读入界符表
		PTfile >> PT[PT_line];
		PT_line++;
		PTfile.get(out);
	}
	PTfile.close();
	/*for (int i = 0; i < PT_line; i++) {
		cout << PT[i] << endl;
	}*/

	ifstream ITfile;
	ITfile.open("测试3标识符表.txt");
	int IT_line = 0;
	while (ITfile.peek() != EOF) {   //从文件中读入标识符表
		ITfile >> IT[IT_line];
		IT_line++;
		ITfile.get(out);
	}
	ITfile.close();
	/*for (int i = 0; i < IT_line; i++) {
		cout << IT[i] << endl;
	}*/

	ifstream CHfile;
	CHfile.open("测试4字符表.txt");
	int CH_line = 0;
	while (CHfile.peek() != EOF) {   //从文件中读入字符表
		CHfile >> CH[CH_line];
		CH_line++;
		CHfile.get(out);
	}
	CHfile.close();
	/*for (int i = 0; i < CH_line; i++) {
	cout << CH[i] << endl;
	}*/

	ifstream CTfile;
	CTfile.open("测试5常数表.txt");
	int CT_line = 0;
	while (CTfile.peek() != EOF) {   //从文件中读入常数表
		CTfile >> CT[CT_line];
		CT_line++;
		CTfile.get(out);
	}
	CTfile.close();
	/*for (int i = 0; i < CT_line; i++) {
	cout << CT[i] << endl;
	}*/

	/*cout << "填写符号表前:" << endl;
	cout << "name" << "	" << "type" << "	" << "cat" << "	" << "addr" << endl;
	for (int i = 0; i < SYNBL_line; i++) {
		cout << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
	}
	*/
	token_to_SYNBL(token_line, SYNBL_line1);
	/*cout << "填写符号表后:" << endl;
	for (int i = 0; i < SYNBL_line; i++) {
		cout << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
	}
	cout << "type:1(int),2(char)，3(float)" << endl << "cat:1(变量),2(函数)" << endl;*/
	ofstream symbolfile2;   //打开符号表文件
	symbolfile2.open("符号表_优化前.txt");
	int i = 0;
	while (i< SYNBL_line1) {//从文件中读入符号表信息
		symbolfile2 << SYNBL[i].name << "	" << SYNBL[i].type << "	" << SYNBL[i].cat << "	" << SYNBL[i].addr << endl;
		i++;
	}
	symbolfile2.close();
	FUNC();
	/*for (int i = 0; i < four_list_num; i++) {
		cout << four_list[i].op << "	" << four_list[i].arg1 << "	" << four_list[i].arg2 << "	" << four_list[i].result << endl;
	}*/
	ofstream four_file;   //打开中间代码文件
	four_file.open("中间代码_优化前.txt");
	i = 0;
	while (i < four_list_num) {//从文件中读入符号表信息
		four_file << four_list[i].op << "	" << four_list[i].arg1 << "	" << four_list[i].arg2 << "	" << four_list[i].result << endl;
		i++;
	}
	four_file.close();
	//system("pause");
}
