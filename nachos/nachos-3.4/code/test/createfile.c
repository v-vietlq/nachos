#include "syscall.h"
#define maxlen 32
int main()
{
	char* buffer;
	ReadString(buffer, maxlen);
	CreateFile(buffer);
	return 0;	
}
