#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED
#define _CRT_SECURE_NO_WARNINGS��
#pragma warning(disable:4996)��

#include <iostream>
#include <ctype.h>
#include <string>
#include <cstdio>
#include <stdlib.h>
using namespace std;

typedef struct
{
    int type;//1�ؼ��� 2��� 3��ʶ�� 4�ַ� 5����  
    string value;//token��ֵ 
    int i;//��value���ڶ�Ӧ���λ��
} token;

typedef struct
{
    char name[15];
    int type;
    int cat;
    int addr;

}fuhaobiao;

token* scaner(FILE* fp); // ���ļ�ɨ���ȡ
int keyOrIdentifier(string value);  // �ж��ǹؼ��ֻ��Ǳ�ʶ��
#endif // LEX_H_INCLUDED
#include "lex.h"

#define _KEY_WORD_END "waiting for your expanding"  /* ����ؼ��ֽ�����־ */
string KEY_WORDS[] = { "main","return","if","else","for","include","printf","int","char",_KEY_WORD_END };   /*������Ĺؼ��ִ���*/
int line_num = 1;
/* �ж��Ǳ�ʶ�����ǹؼ���*/
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

    /* �жϻس� */
    if (int(ch) == 3)
    {
        line_num++;
        return (mytoken);
    }
    /* �жϿո� */
    else if (isspace(ch))
    {
        return (mytoken);
    }
    /* ��ʶ�����ؼ��� */
    else if (isalpha(ch))    //������ַ�����ĸ
    {
        while (isalpha(ch) || isdigit(ch))
        {
            value = value + ch;
            ch = fgetc(fp);
        }
        fseek(fp, -1, SEEK_CUR);  //����ȡ���ַ��Ȳ�����ĸҲ�������֣���ָ����Ȼָ����ַ�
        mytoken->type = keyOrIdentifier(value);  // �жϻ�ȡ���ַ����ǹؼ��ֻ��Ǳ�ʶ������ȡ����Ӧ������
        mytoken->value = value;
        return(mytoken);
    }

    /* ���� */
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

            /* ����� */
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
            /* ���ź�ע�� */
        case '/':
            ch = fgetc(fp);
            if (ch == '/')
            {
                ch = fgetc(fp);
                while (int(ch) != 10 && ch != EOF) // ����ע�ͷ��ţ���һֱ��ȡ��ֱ�������س�Ϊֹ
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


            /* ��� */
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


            /* ������*/
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
    int over = 1; //����ʷ��������ִ����������ȡ
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
