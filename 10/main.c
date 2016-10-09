#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LINE_COUNTS 10

pthread_mutex_t mutex[3];

typedef struct Indexes {
	int firstIndex;
	int secondIndex;
} Indexes;

Indexes additionalThreadIndexes[3] = {{0, 1}, {2, 0}, {1, 2}};
Indexes mainThreadIndexes[3] = {{2, 0}, {1, 2}, {0, 1}};

void* doIt(void *arg) {
    pthread_mutex_lock(&mutex[0]);
    pthread_mutex_lock(&mutex[2]);

	for (int i = 0; i < LINE_COUNTS; ++i) {
		printf("Additional Thread - %d\n", i);
		
		pthread_mutex_unlock(&mutex[additionalThreadIndexes[i % 3].firstIndex]);
		pthread_mutex_lock(&mutex[additionalThreadIndexes[i % 3].secondIndex]);
	}

    pthread_exit(NULL);
}

int main() {
    for (int k = 0; k < 3; ++k) {
		pthread_mutex_init(&mutex[k], NULL);
	}
    
    pthread_t thread;
    int resultOfThreadCreation = pthread_create(&thread, NULL, doIt, NULL);
    if (0 != resultOfThreadCreation) {
        perror("Error while cretaing thread");
		exit(0);
    }

    pthread_mutex_lock(&mutex[1]);
    pthread_mutex_lock(&mutex[2]);

    usleep(1000);

	for (int i = 0; i < LINE_COUNTS; ++i) {
		
		pthread_mutex_unlock(&mutex[mainThreadIndexes[i % 3].firstIndex]);
		pthread_mutex_lock(&mutex[mainThreadIndexes[i % 3].secondIndex]);
		printf("Main Thread - %d\n", i);
	}
	
	 for (int k = 0; k < 3; ++k) {
		pthread_mutex_destroy(&mutex[k]);
	}
}
