#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LINE_COUNTS 100

pthread_mutex_t mutex;
pthread_cond_t cond;
bool isWorkedFirst = true;

void* doIt(void *arg) {
    pthread_mutex_lock(&mutex);
	for (int i = 0; i < LINE_COUNTS; ++i) {
		while (!isWorkedFirst) {
           pthread_cond_wait(&cond, &mutex);
		}
		
		printf("Additional Thread - %d\n", i);
		isWorkedFirst = false;
		pthread_cond_signal(&cond);
	}
	
	pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t thread;
    int resultOfThreadCreation = pthread_create(&thread, NULL, doIt, NULL);
    if (0 != resultOfThreadCreation) {
        perror("Error while cretaing thread");
		exit(0);
    }

    pthread_mutex_lock(&mutex);
	for (int i = 0; i < LINE_COUNTS; ++i) {
		while (isWorkedFirst) {
            pthread_cond_wait(&cond, &mutex);
		}
		
		printf("Main Thread - %d\n", i);
		isWorkedFirst = true;

        pthread_cond_signal(&cond);
	}
	
	pthread_mutex_unlock(&mutex);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
