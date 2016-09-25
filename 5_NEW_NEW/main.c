#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define EXIT_ERROR -1
#define TIME_TO_SLEEP 2
#define EXECUTE 1

void printInTheEnd(void *arg) {
    printf ("The thread is interrupted\n");
}

void* doSmth(void *arg) {
	pthread_cleanup_push(printInTheEnd, NULL);
	
	while(1) {
		printf ("1");
	}
	
	pthread_cleanup_pop(EXECUTE);
	
	pthread_exit(NULL);
}

int main() {
	pthread_t thread;
	
	int result = pthread_create(&thread, NULL, doSmth, NULL);
	
	if (0 != result) {
		perror("Error while creating thread");
		exit(EXIT_ERROR);
	}
	
	sleep(TIME_TO_SLEEP);
	
	int cancelResult = pthread_cancel(thread);
	
	if (0 != cancelResult) {
		perror("Error while cancelling thread");
	}
	
	pthread_exit(NULL);
}
