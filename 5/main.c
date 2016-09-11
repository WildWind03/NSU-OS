#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define COUNT_OF_THREADS 4
#define ERROR_EXIT -1
#define COUNT_OF_LINES 4
#define LENGTH_OF_LINE 256
#define EXECUTE 1

void printInTheEnd(void *arg) {
    printf ("The thread is interrupted\n");
}

void* printMessage(void *arg) {
    pthread_cleanup_push(printInTheEnd, NULL)
	printf ("%s\n", (char*) arg);
	pthread_cleanup_pop(EXECUTE);
	pthread_exit(NULL);
}

int main() {
	pthread_t thread[COUNT_OF_THREADS];
	//char threadMessages[4][128] = {"Thread 1", "Thread 2", "Thread 3", "Thread 4"};
	char **messageSequence = (char**) malloc(sizeof(char*) * COUNT_OF_LINES);

	for (int k = 0; k < COUNT_OF_LINES; ++k) {
		messageSequence[k] = (char*) malloc(sizeof(char) * LENGTH_OF_LINE);
			messageSequence[k][0] = 'T';
			messageSequence[k][1] = 'r';
			messageSequence[k][2] = 'e';
	}

	messageSequence[0][3] = '0';
    messageSequence[1][3] = '1';
	messageSequence[2][3] = '2';
	messageSequence[3][3] = '3';


	int i;
	for (i = 0; i < COUNT_OF_THREADS; ++i) {
		int resultOfThreadCreation = pthread_create(&thread[i], NULL, printMessage, (void*) messageSequence[i]);

		if (0 != resultOfThreadCreation) {
			perror ("Can't create new thread");
			exit(ERROR_EXIT);
		}
	}

	int k;
	for (k = 0; k < COUNT_OF_THREADS; ++k) {
		pthread_join(thread[k], NULL);
	}

	for (int k = 0; k < COUNT_OF_LINES; ++k) {
        free (messageSequence[k]);
	}

	free(messageSequence);

	pthread_exit(NULL);
}
