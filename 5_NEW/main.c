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
	    
    char **strings = (char**) arg;

    for (int k = 0; k < COUNT_OF_LINES; ++k) {
      	printf ("%s\n", strings[k]);
      	free(strings[k]);
    }

    free(strings);
    
    pthread_cleanup_pop(EXECUTE);
	pthread_exit(NULL);
}

int main() {
	pthread_t thread[COUNT_OF_THREADS];
	char ***messageSequence = (char***) malloc(sizeof(char**) * COUNT_OF_THREADS);

	for (int k = 0; k < COUNT_OF_THREADS; ++k) {
        messageSequence[k] = (char**) malloc(sizeof(char*) * COUNT_OF_LINES);

        for (int i = 0; i < COUNT_OF_LINES; ++i) {
            messageSequence[k][i] = (char*) malloc (sizeof(char) * LENGTH_OF_LINE);
        }
	}

	for (int k = 0; k < COUNT_OF_THREADS; ++k) {
        for (int i = 0; i < COUNT_OF_LINES; ++i) {
            sprintf(messageSequence[k][i], "Thread N %d", k);
        }
	}

	for (int i = 0; i < COUNT_OF_THREADS; ++i) {
		int resultOfThreadCreation = pthread_create(&thread[i], NULL, printMessage, (void*) messageSequence[i]);

		if (0 != resultOfThreadCreation) {
			perror ("Can't create new thread");
			exit(ERROR_EXIT);
		}
	}

    free(messageSequence);

    for (int k = 0; k < COUNT_OF_THREADS; ++k) {
       pthread_join(thread[k], NULL);
   }

	exit(EXIT_SUCCESS);
}
