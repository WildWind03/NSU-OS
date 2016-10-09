#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#define CORRECT_NUMBER_OF_ARGS 2
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define MAX_THREAD_COUNT 30
#define MIN_THREAD_COUNT 1
#define COUNT_OF_ITER 100000

typedef struct InfoForThread {
    int posOfThread;
    int countOfThreads;
    double particularSum;
} InfoForThread;

bool isStop = false;
int maxIterCounts = 0;
int countOfThreads = 0;
pthread_mutex_t myMutex;
pthread_cond_t myCond;
pthread_mutex_t waitCondMutex;


void stopCalculating(int signalNum) {
    isStop = true;
}

void* countSum(void *arg) {
    sigset_t mySigset;
    sigemptyset(&mySigset);
    sigaddset(&mySigset, SIGINT);

    pthread_sigmask(SIG_BLOCK, &mySigset, NULL);

    double sum = 0;

    InfoForThread *infoForThread = (InfoForThread*) arg;


    int k;
    for (k = 0; k < COUNT_OF_ITER || !isStop; ++k) {
        double sign = 1.0;
        if (0 != infoForThread -> posOfThread%2) {
            sign = -1.0;
        }

        sum += sign / ((2 * infoForThread -> posOfThread)+ 1);
        infoForThread -> posOfThread+=infoForThread -> countOfThreads;
    }

    if (isStop) {
        pthread_mutex_lock(&myMutex);
        if (maxIterCounts < k) {
            maxIterCounts = k;
        }

        countOfThreads++;

        if (countOfThreads == infoForThread -> countOfThreads) {
            pthread_cond_broadcast(&myCond);
        } else {
            pthread_cond_wait(&myCond, &myMutex);
        }

        pthread_mutex_unlock(&myMutex);

        for ( ; k < maxIterCounts; ++k) {
            double sign = 1.0;

            if (0 != infoForThread -> posOfThread%2) {
                sign = -1.0;
            }

            sum += sign / ((2 * infoForThread -> posOfThread)+ 1);
            infoForThread -> posOfThread+=infoForThread -> countOfThreads;
        }


    }

    infoForThread -> particularSum = sum;

    pthread_exit(infoForThread);
}

bool isNumber(char *str) {
    for (int k = 0; k < strlen(str); ++k) {
        if (str[k] < '0' || str[k] > '9') {
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (CORRECT_NUMBER_OF_ARGS != argc) {
        perror("Invalid count of args\n");
        exit(EXIT_FAILURE);
    }

    if (!isNumber(argv[1])) {
        perror ("Invalid arg. You must type number of threads\n");
        exit(EXIT_FAILURE);
    }

    int numOfThreads = atoi(argv[1]);

    if (numOfThreads < MIN_THREAD_COUNT || numOfThreads > MAX_THREAD_COUNT) {
        perror("Invalid count of threads");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&myMutex, NULL);
    pthread_mutex_init(&waitCondMutex, NULL);
    pthread_cond_init(&myCond, NULL);

    signal(SIGINT, stopCalculating);

    InfoForThread *infoForThread = (InfoForThread*) malloc (sizeof(InfoForThread) * numOfThreads);
    pthread_t* threads = (pthread_t*) malloc (sizeof(pthread_t) * numOfThreads);

    for (int k = 0; k < numOfThreads; ++k) {
        infoForThread[k].countOfThreads = numOfThreads;
        infoForThread[k].posOfThread = k;
        int resultOfThreadCreating = pthread_create(&threads[k], NULL, countSum, &infoForThread[k]);

        if (0 != resultOfThreadCreating) {
            perror("Can not create new thread");
        }
    }

    for (int k = 0; k < numOfThreads; ++k) {
        pthread_join(threads[k], NULL);
    }

    pthread_mutex_destroy(&myMutex);
    pthread_mutex_destroy(&waitCondMutex);
    pthread_cond_destroy(&myCond);

    double sumOfParticularSums = 0;

    for (int k = 0; k < numOfThreads; ++k) {
        sumOfParticularSums+=infoForThread[k].particularSum;
    }

    printf("%f\n", sumOfParticularSums * 4);

       free(infoForThread);
       free(threads);
}
