#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define PHILOSOPHERS_COUNT 5
#define SPAGHETTI_COUNT 70000
#define TIME_TO_EAT 23
#define TIME_TO_THINK 32

int timeToSleepForPhilosopher = 1;

pthread_t philosophers[PHILOSOPHERS_COUNT];
pthread_mutex_t forks[PHILOSOPHERS_COUNT];

pthread_mutex_t foodLock;

bool isNumber(char *str) {
    for (int k = 0; k < strlen(str); ++k) {
        if (str[k] < '0' || str[k] > '9') {
            return false;
        }
    }

    return true;
}

bool takeFood() {
    pthread_mutex_lock(&foodLock);

    static int countOfFood = SPAGHETTI_COUNT;

    if (countOfFood <= 0) {
        pthread_mutex_unlock(&foodLock);
        return false;
    }

    countOfFood--;

    pthread_mutex_unlock(&foodLock);

    return true;
}

void getFork(int philNum, int forkNum, bool isRightHand) {
    pthread_mutex_lock(&forks[forkNum]);
}

void downForks(int forkNum1, int forkNum2) {
    pthread_mutex_unlock(&forks[forkNum1]);
    pthread_mutex_unlock(&forks[forkNum2]);
}

void* work (void* numOfPhilosopher) {
    int id = (int) numOfPhilosopher;

    int leftFork = id + 1;
    int rightFork = id;

    if (PHILOSOPHERS_COUNT == leftFork) {
        leftFork = 0;
    }

    while(takeFood()) {
        if (leftFork > rightFork) {
            getFork(id, leftFork, false);
            getFork(id, rightFork, true);
        } else {
            getFork(id, rightFork, true);
            getFork(id, leftFork, false);
        }

        usleep(TIME_TO_EAT);

        if (leftFork > rightFork) {
            downForks(leftFork, rightFork);
        } else {
            downForks(rightFork, leftFork);
        }

        usleep(TIME_TO_THINK);
    }
}

int main(int argc, char *argv[]) {
    if (2 != argc) {
        perror ("Invalid count of args. Pleaise, type time to sleep");
        exit(EXIT_FAILURE);
    }

    if (!isNumber(argv[1])) {
        perror("The arg is not a number");
        exit(EXIT_SUCCESS);
    }

    timeToSleepForPhilosopher = atoi(argv[1]);

    pthread_mutex_init(&foodLock, NULL);

    for (int i = 0; i < PHILOSOPHERS_COUNT; ++i) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < PHILOSOPHERS_COUNT; ++i) {
        pthread_create(&philosophers[i], NULL, work, (void *) i);
    }


    for (int i = 0; i < PHILOSOPHERS_COUNT; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    pthread_mutex_destroy(&foodLock);

    for (int k = 0; k < PHILOSOPHERS_COUNT; ++k) {
        pthread_mutex_destroy(&forks[k]);
    }

    printf("There is not food now\n");

    return 0;
}
