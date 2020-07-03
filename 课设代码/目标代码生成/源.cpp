void print_code(Quaternion qua)
{
	putchar('\n');
	/*
	 mov eax, 3
	 add eax, 4
	 mov t+0, eax
	*/
	if (qua.op == '+') {
		printf("        mov eax,%s\n", qua.arg1);
		printf("        add eax,%s\n", qua.arg2);
		printf("        mov %s,eax\n", qua.result);
	}
	else if (qua.op == '-') {
		printf("        mov eax,%s\n", qua.arg1);
		printf("        sub eax,%s\n", qua.arg2);
		printf("        mov %s,eax\n", qua.result);
	}
	/*
	mov eax, 2
	mov ebx, t+0
	mul ebx
	mov t+4, eax
	*/
	else if (qua.op == '*') {
		printf("        mov eax,%s\n", qua.arg1);
		printf("        mov ebx,%s\n", qua.arg2);
		printf("        mul ebx\n");
		printf("        mov %s,eax\n", qua.result);
	}
	else if (qua.op == '/') {//除法的时候不考虑余数
		printf("        mov eax,%s\n", qua.arg1);
		printf("        mov ebx,%s\n", qua.arg2);
		printf("        div ebx\n");
		printf("        mov %s,eax\n", qua.result);
	}
}