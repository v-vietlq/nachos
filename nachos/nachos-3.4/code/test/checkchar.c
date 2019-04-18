#include "syscall.h"

int main()
{
	char c;
	PrintString("Enter of Chacacter: ");
	c = ReadChar();
	PrintChar(c);
	return 0;
}
