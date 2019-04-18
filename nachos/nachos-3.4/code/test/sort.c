/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

int Arr[100];
main()
{
	int n,i,j,temp;

	// Enter Array
	do
	{
	PrintString("Enter number of elements: ");
	n = ReadInt();
	} while (n <= 0);
	for(i = 0; i< n; i++)
	{
	PrintString("Arr[");
	PrintInt(i);
	PrintString("]= ");
	Arr[i] = ReadInt();
	}

	//Bubble Sort
	for(i = 0; i < n - 1 ; i++)
	{
	for(j = 0; j < n - 1 - i; j++)
		{
		if(Arr[j] > Arr[j+1])
			{
				temp = Arr[j];
				Arr[j] = Arr[j+1];
				Arr[j+1] = temp;	
			}
		}
	}

	// Print Result
	
	for(i = 0; i < n; i++)
	{
	PrintInt(Arr[i]);
	PrintString(" ");
	}
	
	return 0;
}
