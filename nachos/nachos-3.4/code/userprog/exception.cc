// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MaxFileLength 32
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------


// Increase Program counter
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    	counter = machine->ReadRegister(NextPCReg);
    	machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}


// Purpose: Copy buffer from User memory space to System memory space.
// Output: buffer[char*]
char* User2System(int virtAddr,int limit)
{
	int i;// index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit +1];//need for terminal string
	if (kernelBuf == NULL)
	return kernelBuf;
	memset(kernelBuf,0,limit+1);
	//printf("\n Filename u2s:");
	for (i = 0 ; i < limit ;i++)
		{
			machine->ReadMem(virtAddr+i,1,&oneChar);
			kernelBuf[i] = (char)oneChar;
			//printf("%c",kernelBuf[i]);
			if (oneChar == 0)
			break;
		}
	return kernelBuf;
}


//Purpose: Copy buffer from System memory space to User memory space
// output: number of byte are copied.
int System2User(int virtAddr,int len,char* buffer)
{

	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0 ;
	do{
		oneChar= (int) buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i ++;
	}while(i < len && oneChar != 0);
	return i;
}


void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

	switch (which) {
	case NoException:
		return;

	case PageFaultException:
		DEBUG('a', "\nNo valid translation found.\n");
		printf("\nNo valid translation found.\n");
		interrupt->Halt();
		break;

	case ReadOnlyException:
		DEBUG('a', "\nWrite attempted to page marked read-only.\n");
		printf("\nWrite attempted to page marked read-only.\n");
		interrupt->Halt();
		break;

	case BusErrorException:
		DEBUG('a', "\nTranslation resulted invalid physical address.\n");
		printf("\nTranslation resulted invalid physical address.\n");
		interrupt->Halt();
		break;

	case AddressErrorException:
		DEBUG('a', "\nUnaligned reference or one that was beyond the end of the address space.\n");
		printf("\nUnaligned reference or one that was beyond the end of the address space.\n");
		interrupt->Halt();
		break;

	case OverflowException:
		DEBUG('a', "\nInteger overflow in add or sub.\n");
		printf("\nInteger overflow in add or sub.\n");
		interrupt->Halt();
		break;

	case IllegalInstrException:
		DEBUG('a', "\nUnimplemented or reserved instr.\n");
		printf("\nUnimplemented or reserved instr.\n");
		interrupt->Halt();
		break;

	case NumExceptionTypes:
		DEBUG('a', "\nNumber exception types.\n");
		printf("\nNumber exception types.\n");
		interrupt->Halt();
		break;

	case SyscallException:
		switch (type){

		case SC_Halt:
			DEBUG('a', "\nShutdown, initiated by user program.\n");
			printf("\nShutdown, initiated by user program.\n");
			interrupt->Halt();
			return;

		case SC_ReadInt:
		{
                   char *buffer;
			buffer= new char[256];  // Create buffer has 256 chacacters
			int numbytes = gSynchConsole->Read(buffer, 255); // read from keyboard number of chacacters

			int first = (buffer[0] == '-') ? 1 : 0; // check number is nagative
			int last = 0; // last chacacter

			int number = 0; // Result
			for(int i = first; i < numbytes ; i++)
			{
				// check 12.000 is true
				if(buffer[i] == '.')
				{
					for(int j = i+1; j< numbytes; j++)
					{
						if(buffer[j] != '0')
						{
						printf("\n The number is wrong.");
						DEBUG('a',"\n The number is wrong.");
						machine->WriteRegister(2, 0);
						IncreasePC();
						delete buffer;
						return;
						}	 
					}
				last = i - 1; // update last
				break;
	
				} else if(buffer[i] < '0' && buffer[i] > '9') // check Invalid.
					{
					printf("\n The number is wrong.");
					DEBUG('a',"\n The number is wrong.");
					machine->WriteRegister(2, 0);
					IncreasePC();
					delete buffer;
					return;
					}
				last = numbytes - 1;		
			} 

			for(int i = first; i <= last; i++ )
			{
			number = number * 10 + buffer[i] - '0'; 
			}
			if(buffer[0] == '-') number = -number;
			machine->WriteRegister(2, number);  // Write number to register $2
			IncreasePC(); // Program counter
			return;
		}

		case SC_PrintInt:
		{	
                    int number = machine->ReadRegister(4); // Read number from register $4
		    if(number == 0) // check 
                    {
                        gSynchConsole->Write("0", 1); 
                        IncreasePC();
                        return;    
                    }
                    int numbytes = 0;
                    int first = 0; 
			
                    if(number < 0) // if number < 0, start first = 1 because buffer[0] = '-' 
                    {
                     number= -number;
		     first = 1;
                    } 	
                    
                    int temp = number; 
		    // count number of chacacters
                    while(temp > 0)
                    {
                        numbytes++;
                        temp /= 10;
                    }
    
		   
                    char* buffer;
                    buffer = new char[256];
                    for(int i = first + numbytes - 1; i >= first; i--)
                    {
                        buffer[i] = (char)((number % 10) + '0' );
                        number /= 10;
                    }
                    if(first == 1) // number < 0
                    {
                        buffer[0] = '-';
			buffer[numbytes + 1] = NULL; // the end of string
                        gSynchConsole->Write(buffer, numbytes + 1); 
                        delete buffer;
                        IncreasePC();
                        return;
                    }
		    else if( first == 0) // number > 0
		    {
		    buffer[numbytes] = NULL;	
                    gSynchConsole->Write(buffer, numbytes);
                    delete buffer;
                    IncreasePC();
                    return; 
	            }      			
					
		}

		case SC_ReadChar:
		{

			char* buffer = new char[255];
			int numBytes = gSynchConsole->Read(buffer, 255);

			if(numBytes > 1) // more than 1 chacacter return 0
			{
				printf("ERROR! Read only one chacracter.");
				DEBUG('a', "ERROR! Read only one chacracter.");
				machine->WriteRegister(2, 0);
			}
			else if(numBytes == 0) // Chacacter is NULL
			{
				printf("ERROR!");
				DEBUG('a',"ERROR!");
				machine->WriteRegister(2, 0);
			}
			else
			{
				char c = buffer[0];
				machine->WriteRegister(2, c);
			}

			delete buffer;
			IncreasePC();
			return;
			break;
		}

		case SC_PrintChar:
		{
			char c = (char)machine->ReadRegister(4); // Read A chacacter from register $4
			gSynchConsole->Write(&c, 1); // write it on the screen
			IncreasePC();
			return;
			break;

		}

		case SC_ReadString:
		{
			int virtAddr, length;
			char* buffer;
			virtAddr = machine->ReadRegister(4); // Read User space adress
			length = machine->ReadRegister(5);  // Read length of buffer
			buffer = User2System(virtAddr, length); //copy buffer from user space to system space 
			gSynchConsole->Read(buffer, length);  // System Read buffer
			System2User(virtAddr, length, buffer); // coppy buffer from sys to user
			delete buffer; 
			IncreasePC();  
			return;
			break;
		}

		case SC_PrintString:
		{
			int virtAddr;
			char* buffer;
			virtAddr = machine->ReadRegister(4); // Read User space adress
			buffer = User2System(virtAddr, 255); // Copy buffer from user space to sys space
			int length = 0;
			while (buffer[length] != NULL) length++; // get length of buffer
			gSynchConsole->Write(buffer, length + 1); // System write buffer on the screen
			delete buffer; 
			IncreasePC();
			return;
			break;
		}

		case SC_Sum:
		{
			int a = machine->ReadRegister(4);
			int b = machine->ReadRegister(5);
			int sum = a + b;
			machine->WriteRegister(2, sum);
			IncreasePC();
			return;
		}

		case SC_CreateFile:
		{
			int virtAddr;
			char *name;
			virtAddr = machine->ReadRegister(4);
			name = User2System(virtAddr, MaxFileLength +1);
			
			if(strlen(name) == 0)
			{
			DEBUG('a',"File is not exist");
			printf("\nFile is not exist\n");
			delete name;
			machine->WriteRegister(2, -1);
			IncreasePC();
			return;
			}
			else if(name == NULL)
			{
			DEBUG('a',"Not enough memory");
			printf("\nNot enough memory\n");
			delete name;
			machine->WriteRegister(2, -1);
			IncreasePC();
			return;
			}
			
			bool create = fileSystem->Create(name, 0);
			if(create == false)
			{
			DEBUG('a',"Not Create File");
			printf("Not Create File");
			delete name;
			machine->WriteRegister(2, -1);
			IncreasePC();
			return;
			}

			DEBUG('a'," Sucess Create File");
			printf("\nSucess Create File\n");
			delete name;
			machine->WriteRegister(2, 0);
			IncreasePC();
			return;	
			break;				

		}
		case SC_Open:
		{
		
			//OpenfileID Open(char *name, int type);
			int virtAddr = machine->ReadRegister(4); // get name address from Register 4
			int type = machine->ReadRegister(5); //get tpye from Register 5.
			char* filename;	
			int slot = fileSystem->findslot();		
			filename = User2System(virtAddr, MaxFileLength); //Coppy Name address from user space to system space.

			//check OS is Open file ?
			if(slot != -1) 
			{
				if(type == 0 || type == 1) // process only type == 0 || type ==1
				{
					// Open file is success
					if((fileSystem->openf[slot] = fileSystem->Open(filename, type)) != NULL) 
					{
						machine->WriteRegister(2, slot); // Return OpenFileID
					}
				}
				else if(type == 2) // Process stdin with type = 2
				{
					machine->WriteRegister(2, 0); // Return OpenFileID
				}	
				else
				{
					machine->WriteRegister(2, 1); // Return OpenFileID
				}
				delete[] filename;
				break;
			}
			machine->WriteRegister(2, -1); //Not open file and return -1
			delete[] filename;
			break;
		}

		case SC_Close:
		{
			int id = machine->ReadRegister(4);
			if(id >= 0 && id <= 9 )
			{
				if(fileSystem->openf[id])
				{
					delete fileSystem->openf[id];
					fileSystem->openf[id] = NULL;
					machine->WriteRegister(2, 0);
					IncreasePC();
					return;	
				}
			}
			machine->WriteRegister(2, -1);
			IncreasePC();
			return;
		}

		case SC_Read:
		{
			int virtAddr = machine->ReadRegister(4); // Read adress of buffer
			int charcount = machine->ReadRegister(5); // Read charcount 
			int id = machine->ReadRegister(6); // Read id of file
			int OldPos;
			int NewPos;
			char *buf;
			// Check id of file
			if (id < 0 || id > 10)
			{
				printf("\nCan not read.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Check survival of file
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nCan not read.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			if (fileSystem->openf[id]->type == 3) // case: read file stdout (type: 3) --> return -1
			{
				printf("\nCan not read file stdout.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Check success --> take position OldPos
			buf = User2System(virtAddr, charcount); // Copy string from User Space to System Space
			// case: read file stdin (type : 2)
			if (fileSystem->openf[id]->type == 2)
			{
				// use read of class SynchConsole to retuen byte real
				int size = gSynchConsole->Read(buf, charcount); 
				System2User(virtAddr, size, buf); // Copy string from System Spaceto User Space with length of buffer is byte real
				machine->WriteRegister(2, size); // return byte real
				delete buf;
				IncreasePC();
				return;
			}
			// case: read file nomal ---> return byte real
			if ((fileSystem->openf[id]->Read(buf, charcount)) > 0)
			{
				// byte real = NewPos - OldPos
				NewPos = fileSystem->openf[id]->GetCurrentPos();
				// Copy string from System Spaceto User Space with length of buffer is byte real
				System2User(virtAddr, NewPos - OldPos, buf); 
				machine->WriteRegister(2, NewPos - OldPos);
			}
			else
			{
				// case: read file empty --> return -2
				//printf("\nRead file empty.");
				machine->WriteRegister(2, -2);
			}
			delete buf;
			IncreasePC();
			return;
		}
		case SC_Write:
		{
			int virtAddr = machine->ReadRegister(4); //read adress of buffer
			int charcount = machine->ReadRegister(5); // Read charcount
			int id = machine->ReadRegister(6); // Read is of file
			int OldPos;
			int NewPos;
			char *buf;
			// Check id of file
			if (id < 0 || id > 10)
			{
				printf("\nCan not write.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Check survival of file
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nCan not write.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// case: write file only read (type: 1) or file stdin (type : 2) return -1
			if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2)
			{
				printf("\ncan not write because file only read or file stdin.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Check success--> take position OldPos
			buf = User2System(virtAddr, charcount);  // Copy string from User Space to System Space  buffer with lenght is charcount
			// case:write file read & write (type: 0) --> return byte real
			if (fileSystem->openf[id]->type == 0)
			{
				if ((fileSystem->openf[id]->Write(buf, charcount)) > 0)
				{
					// byte real = NewPos - OldPos
					NewPos = fileSystem->openf[id]->GetCurrentPos();
					machine->WriteRegister(2, NewPos - OldPos);
					delete buf;
					IncreasePC();
					return;
				}
			}
			if (fileSystem->openf[id]->type == 3) // case: write file stdout (type: 3)
			{
				int i = 0;
				while (buf[i] != 0 && buf[i] != '\n') // while to write until meet '\n'
				{
					gSynchConsole->Write(buf + i, 1); //use Write of class SynchConsole 
					i++;
				}
				buf[i] = '\n';
				gSynchConsole->Write(buf + i, 1); // Write character '\n'
				machine->WriteRegister(2, i - 1); // return byte real
				delete buf;
				IncreasePC();
				return;
			}
		}
		// int Seek(int pos, OpenFileID id);
		case SC_Seek:
		{
			int pos = machine->ReadRegister(4);
			int id = machine-> ReadRegister(5);
			if(id < 0 || id > 9)
			{
			DEBUG('a',"Outside the deciption table\n");
			printf("Outside the deciption table\n");
			machine->WriteRegister(2, -1);
			IncreasePC();
			return;
			}
			else if(fileSystem->openf[id] == NULL)
			{
			DEBUG('a',"file is not exist.\n");
			printf("file is not exist.\n");
			machine->WriteRegister(2, -1);
			IncreasePC();
			return;
			}
			else if(id == 0 || id == 1)
			{
			DEBUG('a',"Not seek file in console.\n");
			printf("Not seek file in console.\n");
			machine->WriteRegister(2, -1);
			IncreasePC();
			}
			int seekpos;
			if(pos == -1) seekpos = fileSystem->openf[id]->Length();
			else seekpos = pos;

			if(seekpos < 0 || seekpos > fileSystem->openf[id]->Length())
			{
			DEBUG('a',"pos is not avaid.\n");
			printf("pos is not avaid.\n");
			machine->WriteRegister(2, -1);
			IncreasePC();
			return;
			}
			else
			{
			fileSystem->openf[id]->Seek(seekpos);
			machine->WriteRegister(2, seekpos);
			IncreasePC();
			return;
			}
		} 

		default:
			break;
		}
		IncreasePC();
	}
}

