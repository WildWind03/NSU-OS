#define LINE_COUNTS 10

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* doIt(void *arg) {
	int i;
	for (i = 0; i < LINE_COUNTS; ++i) {
		printf("The second thread - %d\n", i);
	}
	
	pthread_exit(NULL);
}

int main() {
	pthread_t thread;
	int resultOfThreadCreation = pthread_create(&thread, NULL, doIt, NULL);
	
	if (0 != resultOfThreadCreation) {
		perror("Error while cretaing thread");
		exit(0);
	}
	
	pthread_join(thread, NULL);
	
	int i;
	for (i = 0; i < LINE_COUNTS; ++i) {
		printf("Main Thread - %d\n", i);
	}
	
	pthread_exit(NULL);
}
