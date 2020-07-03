#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED
#define _CRT_SECURE_NO_WARNINGS；
#pragma warning(disable:4996)；

#include <iostream>
#include <ctype.h>
#include <string>
#include <cstdio>
#include <stdlib.h>
using namespace std;

typedef struct
{
    int type;//1关键字 2界符 3标识符 4字符 5常数  
    string value;//token的值 
    int i;//该value所在对应表的位置
} token;

typedef struct
{
    char name[15];
    int type;
    int cat;
    int addr;

}fuhaobiao;

token* scaner(FILE* fp); // 对文件扫描读取
int keyOrIdentifier(string value);  // 判断是关键字还是标识符
#endif // LEX_H_INCLUDED
#include "lex.h"

#define _KEY_WORD_END "waiting for your expanding"  /* 定义关键字结束标志 */
string KEY_WORDS[] = { "main","return","if","else","for","include","printf","int","char",_KEY_WORD_END };   /*可扩充的关键字词组*/
int line_num = 1;
/* 判断是标识符还是关键字*/
int keyOrIdentifier(string value)
{
    int i = 0;
    while (KEY_WORDS[i] != _KEY_WORD_END)
    {
        if (KEY_WORDS[i] == value)
        {
            return 1;
        }
        i = i + 1;
    }
    return 3;
}

token* scaner(FILE* fp)
{
    char ch;
    string value = "";
    token* mytoken = new token;
    mytoken->type = 0;
    mytoken->value = "";

    ch = fgetc(fp);

    /* 判断回车 */
    if (int(ch) == 3)
    {
        line_num++;
        return (mytoken);
    }
    /* 判断空格 */
    else if (isspace(ch))
    {
        return (mytoken);
    }
    /* 标识符及关键字 */
    else if (isalpha(ch))    //如果首字符是字母
    {
        while (isalpha(ch) || isdigit(ch))
        {
            value = value + ch;
            ch = fgetc(fp);
        }
        fseek(fp, -1, SEEK_CUR);  //当获取的字符既不是字母也不是数字，则指针依然指向该字符
        mytoken->type = keyOrIdentifier(value);  // 判断获取的字符串是关键字还是标识符，获取它对应的类型
        mytoken->value = value;
        return(mytoken);
    }

    /* 数字 */
    else if (isdigit(ch))
    {
        while (isdigit(ch))
        {
            value = value + ch;
            ch = fgetc(fp);
        }
        fseek(fp, 0, SEEK_CUR);
        mytoken->type = 4;
        mytoken->value = value;
        return(mytoken);
    }
    else
        switch (ch)
        {

            /* 运算符 */
        case '+':
            ch = fgetc(fp);
            if (ch == '+')
            {
                mytoken->type = 4;
                mytoken->value = "++";
                return(mytoken);
            }
            fseek(fp, 0, SEEK_CUR);
            mytoken->type = 4;
            mytoken->value = "+";
            return(mytoken);
            break;
        case '-':
            ch = fgetc(fp);
            if (ch == '-')
            {
                mytoken->type = 4;
                mytoken->value = "--";
                return(mytoken);
            }
            fseek(fp, 0, SEEK_CUR);
            mytoken->type = 4;
            mytoken->value = "-";
            return(mytoken);
            break;
        case '*':
            mytoken->type = 4;
            mytoken->value = "*";
            return(mytoken);
            break;
            /* 除号和注释 */
        case '/':
            ch = fgetc(fp);
            if (ch == '/')
            {
                ch = fgetc(fp);
                while (int(ch) != 10 && ch != EOF) // 发现注释符号，则一直读取，直到遇到回车为止
                {
                    ch = fgetc(fp);
                }
                if (int(ch) == 3)
                {
                    line_num++;
                }
            }
            fseek(fp, 0, SEEK_CUR);
            mytoken->type = 4;
            mytoken->value = "/";
            return(mytoken);
            break;
        case '(':
            mytoken->type = 2;
            mytoken->value = "(";
            return(mytoken);
            break;
        case '=':
            ch = fgetc(fp);
            if (ch == '=')
            {
                mytoken->type = 4;
                mytoken->value = "==";
                return(mytoken);
            }
            fseek(fp, 0, SEEK_CUR);
            mytoken->type = 4;
            mytoken->value = "=";
            return(mytoken);
            break;
        case '!':
            ch = fgetc(fp);
            if (ch == '=')
            {
                mytoken->type = 4;
                mytoken->value = "!=";
                return(mytoken);
            }
            fseek(fp, 0, SEEK_CUR);
            mytoken->type = -1;
            mytoken->value = "ERROR";
            return(mytoken);
            break;
        case '>':
            ch = fgetc(fp);
            if (ch == '=')
            {
                mytoken->type = 4;
                mytoken->value = ">=";
                return(mytoken);
            }
            fseek(fp, 0, SEEK_CUR);
            mytoken->type = 4;
            mytoken->value = ">";
            return(mytoken);
            break;
        case '<':
            ch = fgetc(fp);
            if (ch == '=')
            {
                mytoken->type = 4;
                mytoken->value = "<=";
                return(mytoken);
            }
            fseek(fp, 0, SEEK_CUR);
            mytoken->type = 4;
            mytoken->value = "<";
            return(mytoken);
            break;


            /* 界符 */
        case ')':
            mytoken->type = 2;
            mytoken->value = ")";
            return(mytoken);
            break;
        case '[':
            mytoken->type = 2;
            mytoken->value = "[";
            return(mytoken);
            break;
        case ']':
            mytoken->type = 2;
            mytoken->value = "]";
            return(mytoken);
            break;
        case '{':
            mytoken->type = 2;
            mytoken->value = "{";
            return(mytoken);
            break;
        case '}':
            mytoken->type = 2;
            mytoken->value = "}";
            return(mytoken);
            break;
        case ',':
            mytoken->type = 2;
            mytoken->value = ",";
            return(mytoken);
            break;
        case ':':
            mytoken->type = 2;
            mytoken->value = ":";
            return(mytoken);
            break;
        case ';':
            mytoken->type = 2;
            mytoken->value = ";";
            return(mytoken);
            break;
        case '"':
            mytoken->type = 2;
            mytoken->value = "\"";
            return(mytoken);
            break;


            /* 结束符*/
        case EOF:
            mytoken->type = 1000;
            mytoken->value = "OVER";
            return(mytoken);
            break;
        }


}
#include "lex.h"

int main()
{
    token* onetoken = new token;
    FILE* fp;
    int over = 1; //如果词法解析出现错误，则结束读取
    if ((fp = fopen("ceshi.txt", "rt")) == NULL) {
        cout << "Cannot open file, press any key to exit!" << endl;
    }
    while (over < 1000 && over != -1)
    {
        onetoken = scaner(fp);
        if (onetoken->type < 1000 && onetoken->type>0)
            cout << "(" << onetoken->type << "," << onetoken->value << ")" << endl;
        over = onetoken->type;
    }
    fclose(fp);
    system("pause");

    return 0;
}
