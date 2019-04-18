//Compile: gcc -o vd vd.c -lpthread

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
sem_t mutexH, mutexO;

/* This is our thread function.  It is like main(), but for a thread*/
void *MakeH(void *arg)
{
	printf("H\n");
	sem_post(&mutexH);
	sem_post(&mutexH);
	return NULL;
}

void *MakeO(void *arg)
{
	printf("O\n");
	sem_post(&mutexO); //Up(ba)
	return NULL;
}

void *MakeH2O(void *arg)
{
	sem_wait(&mutexH);
	sem_wait(&mutexH);
	sem_wait(&mutexO);
	printf("=>H2O\n");
	return NULL;
}
int main(void)
{
	
	pthread_t pth1, pth2, pth3, pth4;	// this is our thread identifier

	pthread_create(&pth1,NULL,MakeH,NULL);
	pthread_create(&pth4,NULL,MakeH,NULL);
	pthread_create(&pth2,NULL,MakeO,NULL);
	pthread_create(&pth3,NULL,MakeH2O,NULL);
	
	pthread_join(pth1,NULL);
	pthread_join(pth2,NULL);
	pthread_join(pth3,NULL);
	pthread_join(pth4,NULL);
	
	return 0;
}
