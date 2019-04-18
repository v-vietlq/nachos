#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

sem_t mutex;
int x = 0;

void *threadFunc(void *arg)
{
	do
	{
		sem_wait(&mutex); // down
		x = x + 1;
		if(x == 20) 
		{	
			sleep(1);
			x = 0;
		}
		else if(x > 20)
		{
		printf("%d\n", x);
		break;
		}
		printf("%d\n", x);
		sem_post(&mutex); // up
		
	}while(1);
	return NULL;
}

int main(void)
{
	sem_init(&mutex, 0, 1);
	pthread_t pth1, pth2;
	int x = 0;
	pthread_create(&pth2,NULL,threadFunc,NULL);
	pthread_create(&pth1,NULL,threadFunc,NULL);
	
	pthread_join(pth1,NULL);
	pthread_join(pth2,NULL);
	sem_destroy(&mutex);
	return 0;
}
