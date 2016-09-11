#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CORRECT_NUMBER_OF_ARGS 2
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define MAX_THREAD_COUNT 30
#define MIN_THREAD_COUNT 1

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
    }


}
