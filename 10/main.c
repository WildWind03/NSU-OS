#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LINE_COUNTS 10

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;

void* doIt(void *arg) {

    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex3);

	for (int i = 0; i < LINE_COUNTS; ++i) {
		printf("Additional Thread - %d\n", i);

        pthread_mutex_unlock(&mutex1);
        pthread_mutex_lock(&mutex2);

        printf("Additional Thread - %d\n", i);

        pthread_mutex_unlock(&mutex3);
        pthread_mutex_lock(&mutex1);

        printf("Additional Thread - %d\n", i);

        pthread_mutex_unlock(&mutex2);
        pthread_mutex_lock(&mutex3);
	}

    pthread_exit(NULL);
}

int main() {
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_mutex_init(&mutex3, NULL);

    pthread_t thread;
    int resultOfThreadCreation = pthread_create(&thread, NULL, doIt, NULL);
    if (0 != resultOfThreadCreation) {
        perror("Error while cretaing thread");
		exit(0);
    }

    pthread_mutex_lock(&mutex2);

    usleep(1000);

	for (int i = 0; i < LINE_COUNTS; ++i) {
        pthread_mutex_lock(&mutex1);
		printf("Main Thread - %d\n", i);

        pthread_mutex_unlock(&mutex2);
        pthread_mutex_lock(&mutex3);

        printf("Main Thread - %d\n", i);

        pthread_mutex_unlock(&mutex1);
        pthread_mutex_lock(&mutex2);

        printf("Main Thread - %d\n", i);

        pthread_mutex_unlock(&mutex3);
	}

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_mutex_destroy(&mutex3);
}
