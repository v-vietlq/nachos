#include "syscall.h"

int main()
{
	int stdin, stdout, srcId, desId, size, i;
	char srcFile[256], desFile[256], c;
	stdin = Open("stdin", 2);
	stdout = Open("stdout", 3);
	if (stdin != -1 && stdout != -1)
	{
		Write("\t\t---- COPY PROGRAM ----\n\n", 50, stdout);
		Write("\t\tUser enter name of source file: ", 50, stdout);
		Read(srcFile, 255, stdin);
		Write("\t\tUser enter name of destination file: ", 50, stdout);
		Read(desFile, 255, stdin);
		srcId = Open(srcFile, 1);
		if (srcId != -1)
		{
			desId = CreateFile(desFile);
			Close(desId);
			desId = Open(desFile, 0);
			if (desId != -1)
			{
				size = Seek(-1, srcId);
				Seek(0, srcId);
				for (i = 0; i < size; i++)
				{
					Read(&c, 1, srcId);
					Write(&c, 1, desId);
				}
				Close(desId);
			}
			Close(srcId);
		}
		else
			Write("\t\tMo file nguon khong thanh cong", 40, stdout);
	}
	Halt();
	return 0;
}
