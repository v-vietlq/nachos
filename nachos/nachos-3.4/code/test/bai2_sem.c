#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

sem_t mutexab, mutexba;

void *ProcessP1(void *arg)
{

	printf("A1\n");
	sem_post(&mutexba); //Up(ba)
	sem_wait(&mutexab); //Down(ab)
	printf("A2\n");
	return NULL;
}

void *ProcessP2(void *arg)
{
	
	printf("B1\n");
	sem_post(&mutexab); //Up(ab)
	sem_wait(&mutexba); //Down(ba)
	printf("B2\n");
	return NULL;
}

int main()
{
	sem_init(&mutexab, 1, 0);
	sem_init(&mutexba, 1, 0);
	pthread_t pth1, pth2;

	
	pthread_create(&pth1,NULL,ProcessP1,NULL);
	pthread_create(&pth2,NULL,ProcessP2,NULL);
	
	pthread_join(pth2,NULL);
	pthread_join(pth1,NULL);
	
	sem_destroy(&mutexab);
	sem_destroy(&mutexba);
	return 0;
}
