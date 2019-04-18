#include "syscall.h"
#include "copyright.h"
#define maxlen 255
int main()
{

	int stdin, stdout;
	char* buffer;
	PrintString("\n-------ECHO---------\n");
	PrintString("\n stdin: ");
	stdin = Open("stdin", 2); // Open file stdin;
	if(stdin != -1) // check decription table has NULL
	{
		int len = Read(buffer, maxlen, stdin);
		if(len != -1 && len != -2) // check read
		{
			stdout = Open("stdout", 3);
			if(stdout != -1) // check decription table has NULL
			{
			PrintString("\nstdout: ");
			Write(buffer, maxlen, stdout);
			Close(stdout); 
			}
		}
	Close(stdin);
	}
	return 0;
}
