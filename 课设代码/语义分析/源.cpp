#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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

symbol SYNBL[1000];		//建立符号表
token token_list[1000];  //建立token序列
string KT[50];
string PT[50];
string IT[1000];

bool Isdata_type(string word) {  //判断是否为数据类型
	for (int i = 0; i < 3; i++) {
		if (data_type[i] == word) return true;
	}
	return false;
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
		else return 0;
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
		else return 0;
	}
	case 3:return 0;  //为后续函数参数留的状态
	case 4:return 0;
	case 5:return 0;
	default:return 0;
	}
}

void token_sacnning(int tline, int sline) {  //token扫描器，填写符号表
	token last_data_type;  //临时储存关键token信息
	string last_symbol_name;   //记录上一个读取的标识符
	int offset = 0;		//记录域宽
	int state = 0;		//状态
	int last_state = 0; //前一个状态
	int lookup[6][6] = { 1,0,2,3,0,0,
						4,0,2,0,0,0,
						0,5,0,0,0,0,
						0,0,1,0,0,0,
						0,0,0,0,0,0,
						0,0,0,0,0,0 }; //lookup[当前状态][前一状态]{操作编号}
	int i = 0;  //token行数
	while (i < tline) {   //逐一读取token
		last_state = state;
		state = state_tran(state, i);  //状态转换
		switch (lookup[state][last_state]) {
		case 0: {
			cout << "err!" << endl;
			break;
		}
		case 1: {  //无操作
			break;
		}
		case 2: {  //变量填表
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
					SYNBL[j].cat = 1;//1是变量种类
				}
			}
			break;
		}
		case 3: {  //函数填表
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
					SYNBL[j].cat = 2;//1是函数种类
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

int main() {
	ifstream symbolfile;   //打开符号表文件
	symbolfile.open("测试符号表（词法）.txt");
	int SYNBL_line = 0;
	while (symbolfile.eof() == 0) {//从文件中读入符号表信息
		symbolfile >> SYNBL[SYNBL_line].name >> SYNBL[SYNBL_line].type >> SYNBL[SYNBL_line].cat >> SYNBL[SYNBL_line].addr;
		SYNBL_line++;
	}
	symbolfile.close();

	ifstream tokenfile;  //打开token序列文件
	tokenfile.open("测试token.txt");
	int token_line = 0;
	while (tokenfile.eof() == 0) {  //从文件中读入token序列信息
		tokenfile >> token_list[token_line].code >> token_list[token_line].value;
		token_line++;
	}
	tokenfile.close();

	ifstream KTfile;
	KTfile.open("测试1关键字表.txt");
	int KT_line = 0;
	while (KTfile.eof() == 0) {   //从文件中读入关键字表
		KTfile >> KT[KT_line];
		KT_line++;
	}
	KTfile.close();

	ifstream PTfile;
	PTfile.open("测试2界符表.txt");
	int PT_line = 0;
	while (PTfile.eof() == 0) {   //从文件中读入界符表
		PTfile >> PT[PT_line];
		PT_line++;
	}
	PTfile.close();

	ifstream ITfile;
	ITfile.open("测试3标识符表.txt");
	int IT_line = 0;
	while (ITfile.eof() == 0) {   //从文件中读入标识符表
		ITfile >> IT[IT_line];
		IT_line++;
	}
	ITfile.close();

	system("pause");
	return 0;
}