#define LINE_COUNTS 10
#define COUNT_OF_THREADS_WHICH_WRITE 1
#define SEM_FOR_THREADS 0

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t sem1;
sem_t sem2;

void* doIt(void *arg) {
	int i;
	for (i = 0; i < LINE_COUNTS; ++i) {
        sem_wait(&sem2);
		printf("The second thread - %d\n", i);
		sem_post(&sem1);
	}

	pthread_exit(NULL);
}

int main() {
	pthread_t thread;
	sem_init(&sem1, SEM_FOR_THREADS, 0);
	sem_init(&sem2, SEM_FOR_THREADS, 1);

	int resultOfThreadCreation = pthread_create(&thread, NULL, doIt, NULL);

	if (0 != resultOfThreadCreation) {
		perror("Error while cretaing thread");
		exit(0);
	}

	int i;
	for (i = 0; i < LINE_COUNTS; ++i) {
        sem_wait(&sem1);
		printf("Main Thread - %d\n", i);
		sem_post(&sem2);
	}

	sem_destroy(&sem1);
	sem_destroy(&sem2);

	pthread_exit(NULL);

}
