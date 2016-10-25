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

typedef struct InfoForCreateModuleThread {
  sem_t *moduleSem;
  sem_t *semA;
  sem_t *semB;
} InfoForCreateModuleThread;

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

void* createModule(void *arg) {
    InfoForCreateModuleThread *infoForCreateModuleThread = (InfoForCreateModuleThread*) arg;

    sem_t *semA = infoForCreateModuleThread -> semA;
    sem_t *semB = infoForCreateModuleThread -> semB;
    sem_t *moduleSem = infoForCreateModuleThread -> moduleSem;

    for (int k = 0; k < COUNT_OF_DEVICES_THAT_HAVE_TO_BE_PRODUCED; ++k) {
      sem_wait(semA);
      sem_wait(semB);
      sem_post(moduleSem);
      printf("New module was created!\n");
    }
}

int main() {
  InfoForThread *infoForThread = (InfoForThread*) malloc (sizeof(InfoForThread) * COUNT_OF_PARTS_OF_DEVICE);
  pthread_t thread[COUNT_OF_PARTS_OF_DEVICE];
  pthread_t moduleThread;
  sem_t moduleSem;
  sem_init(&moduleSem, SEM_FOR_THREADS, 0);

  for (int k = 0; k < COUNT_OF_PARTS_OF_DEVICE; ++k) {
    infoForThread[k].sem = (sem_t*) malloc (sizeof(sem_t));
    infoForThread[k].timeToSleep = k + 1;
    sem_init(infoForThread[k].sem, SEM_FOR_THREADS, 0);
    pthread_create(&thread[k], NULL, work, &infoForThread[k]);
  }

  InfoForCreateModuleThread *infoForCreateModuleThread = (InfoForCreateModuleThread*) malloc (sizeof(InfoForCreateModuleThread));
  infoForCreateModuleThread -> semA = infoForThread[0].sem;
  infoForCreateModuleThread -> semB = infoForThread[1].sem;
  infoForCreateModuleThread -> moduleSem = &moduleSem;

  pthread_create(&moduleThread, NULL, createModule, infoForCreateModuleThread);

  int countOfDevicesThatHaveToBeProduced = COUNT_OF_DEVICES_THAT_HAVE_TO_BE_PRODUCED;

  while(--countOfDevicesThatHaveToBeProduced >= 0) {
    sem_wait(infoForThread[2].sem);
    sem_wait(&moduleSem);
    printf ("New device was created!\n");
  }

  for (int k = 0; k < COUNT_OF_PARTS_OF_DEVICE; ++k) {
    pthread_cancel(thread[k]);
  }

  pthread_cancel(moduleThread);
  pthread_join(moduleThread, NULL);
  free(infoForCreateModuleThread);

  for (int k = 0; k < COUNT_OF_PARTS_OF_DEVICE; ++k) {
    pthread_join(thread[k], NULL);
    sem_destroy(infoForThread[k].sem);
    free (infoForThread[k].sem);
  }

  sem_destroy(&moduleSem);

  free(infoForThread);

}
