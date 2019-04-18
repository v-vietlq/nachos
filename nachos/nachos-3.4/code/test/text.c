#include "syscall.h"

int main()
{
	char *buffer;
	PrintString("Enter string: ");
	ReadString(buffer, 255);
	PrintString(buffer);
	return 0;
}
