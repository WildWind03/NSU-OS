#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <dos.h>

#define EXIT_ERROR -1
#define TIME_TO_SLEEP 2

void* doSmth(void *arg) {
	while(1) {
		printf ("I'm working");
	}
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
