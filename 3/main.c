#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define COUNT_OF_THREADS 4
#define ERROR_EXIT -1

void* printMessage(void *arg) {
	printf ("%s\n", (char*) arg);
	pthread_exit(NULL);
}

int main() {
	pthread_t thread[COUNT_OF_THREADS];
	char threadMessages[4][128] = {"Thread 1", "Thread 2", "Thread 3", "Thread 4"};
	
	int i;
	for (i = 0; i < COUNT_OF_THREADS; ++i) {
		int resultOfThreadCreation = pthread_create(&thread[i], NULL, printMessage, (void*) &threadMessages[i]);
		
		if (0 != resultOfThreadCreation) {
			perror ("Can't create new thread");
			exit(ERROR_EXIT);
		}
	}
	
	int k;
	for (k = 0; k < COUNT_OF_THREADS; ++k) {
		pthread_join(thread[k], NULL);
	}
	
	pthread_exit(NULL);
}
