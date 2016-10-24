#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define SEM_FOR_THREADS 0
#define COUNT_OF_DEVICES_THAT_HAVE_TO_BE_PRODUCED 10

typedef struct InfoForThread {
  sem_t *sem;
  int timeToSleep;
} InfoForThread;

const int COUNT_OF_PARTS_OF_DEVICE = 3;

void* work(void *arg) {
    InfoForThread *infoForThread = (InfoForThread*) arg;
    sem_t *partOfDevice = infoForThread -> sem;
    int timeToSleep = infoForThread -> timeToSleep * 1000000;

    while(1) {
      sem_post(partOfDevice);
      printf ("New detail of type %d was created!\n", infoForThread -> timeToSleep);
      usleep(timeToSleep);
    }
}

int main() {
  InfoForThread *infoForThread = (InfoForThread*) malloc (sizeof(InfoForThread) * COUNT_OF_PARTS_OF_DEVICE);
  pthread_t thread[COUNT_OF_PARTS_OF_DEVICE];

  for (int k = 0; k < COUNT_OF_PARTS_OF_DEVICE; ++k) {
    infoForThread[k].sem = (sem_t*) malloc (sizeof(sem_t));
    infoForThread[k].timeToSleep = k + 1;
    sem_init(infoForThread[k].sem, SEM_FOR_THREADS, 0);
    pthread_create(&thread[k], NULL, work, &infoForThread[k]);
  }

  int countOfDevicesThatHaveToBeProduced = COUNT_OF_DEVICES_THAT_HAVE_TO_BE_PRODUCED;

  while(--counter >= 0) {
    for (int k = 0; k < COUNT_OF_PARTS_OF_DEVICE; ++k) {
      sem_wait(infoForThread[k].sem);
    }
    printf ("New device was created!\n");
  }

  for (int k = 0; k < COUNT_OF_PARTS_OF_DEVICE; ++k) {
    pthread_cancel(thread[k]);
  }

  for (int k = 0; k < COUNT_OF_PARTS_OF_DEVICE; ++k) {
    pthread_join(thread[k], NULL);
    sem_destroy(infoForThread[k].sem);
    free (infoForThread[k].sem);
  }

  free(infoForThread);

}
