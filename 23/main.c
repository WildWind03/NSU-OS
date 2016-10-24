#include "list.c"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

queue myqueue;

void* work1(void *arg) {
    for (int k = 0; k < 10; ++k) {
        mymsgput(&myqueue, "Tread 1 wrote");
        usleep(1000000);
    }
}

void* work2(void *arg) {
  for (int k = 0; k < 10; ++k) {
      mymsgput(&myqueue, "Tread 2 wrote");
      usleep(2000000);
  }
}

void* work3(void *arg) {
  for (int k = 0; k < 10; ++k) {
    char buffer[20];
    mymsgget(&myqueue, buffer, 20);
    printf ("'%s' was written by thread 3\n", buffer);
    usleep(500000);
  }
}

void* work4(void *arg) {
  for (int k = 0; k < 10; ++k) {
    char buffer[20];
    mymsgget(&myqueue, buffer, 20);
    printf ("'%s' was written by thread 4\n", buffer);
    usleep(800000);
  }
}

int main() {
  mymsginit(&myqueue);
  pthread_t pthread1, pthread2, pthread3, pthread4;

  pthread_create (&pthread1, NULL, work1, NULL);
  pthread_create (&pthread2, NULL, work2, NULL);
  pthread_create (&pthread3, NULL, work3, NULL);
  pthread_create (&pthread4, NULL, work4, NULL);

  pthread_join(pthread1, NULL);
  pthread_join(pthread2, NULL);
  pthread_join(pthread3, NULL);
  pthread_join(pthread4, NULL);
  mymsgdestroy(&myqueue);
  return 0;
}
