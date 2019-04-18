#include "syscall.h"
#define maxlen 32
int main()
{
	int Id1,Id2, size, i;
	char* temp;
	char c;
	char filename1[maxlen]; 
	char filename2[maxlen];
	PrintString("\n------Append--------\n");
	
	PrintString("\n Enter of file name: ");
	ReadString(filename1, maxlen);
	PrintString("\n Enter of file name: ");
	ReadString(filename2, maxlen);
	CreateFile(filename2);
	Id1 = Open(filename1, 1); // Only Read
	Id2 = Open(filename2, 0); // Read and write
	if(Id1 != -1 && Id2 != -1) // check isOpen
	{
		size = Seek(-1, Id1); // Seek the end of file (size of file)
		Seek(0, Id1); // Seek the begin of file
		for(i = 0; i < size; i++)
		{
			Read(&c, 1, Id1);
			temp[size -1 - i] = c;
		}
		Write(temp, size, Id2);
	Close(Id1);
	Close(Id2);
	}
	return 0;
}
