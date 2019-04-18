//Compile: gcc -o vd vd.c -lpthread

#include <pthread.h>
#include <stdio.h>

int taikhoan = 900;

/* This is our thread function. It is like main(), but for a thread*/
void *threadFunc(void *arg)
{
	int tienrut = (int) arg;

	if (taikhoan - tienrut > 0)
	{
		sleep(1);
		taikhoan = taikhoan - tienrut;
		printf("taikhoan = %d \n", taikhoan);
		
	}
	else //if (taikhoan - tienrut < = 0)
		printf ("Khong the rut tien!\n");
	
	return NULL;
}

int main(void)
{
	pthread_t pth1, pth2;	// These are our thread identifiers
	int tienrut1 = 600;
	int tienrut2 = 400;
	
	pthread_create(&pth1, NULL, threadFunc, (void*)tienrut1);
	pthread_create(&pth2, NULL, threadFunc, (void*)tienrut2);

	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);
	
	return 0;
}
