#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define SEM_FOR_THREADS 0

#define MAX_LENGTH_OF_STRING 80

#define MAX_SIZE 10

#define ALLOCATE_ERROR -1
#define EMPTY_LIST_ERROR -2
#define FULL_LIST_ERROR -3

typedef struct node {
  node *next;
  node *prev;
  char *text;
} Node;

typedef struct queue {
  node *lastNode;
  node *firstNode;
  sem_t freeSem;
  sem_t busySem;
  sem_t sem;
  size_t size;
  bool isDestroyed;
} queue;

void mymsginit(queue *myqueue) {
  myqueue -> size = 0;
  sem_init(&myqueue -> sem, SEM_FOR_THREADS, 1);
  sem_init(&myqueue -> freeSem, SEM_FOR_THREADS, MAX_SIZE);
  sem_init(&myqueue -> busySem, SEM_FOR_THREADS, 0);
  myqueue -> lastNode = NULL;
  myqueue -> firstNode = NULL;
  myqueue -> isDestroyed = false;
}

void mymsgdrop(queue *myqueue) {
    myqueue -> isDestroyed = true;
    sem_post(&myqueue -> freeSem);
    sem_post(&myqueue -> busySem);
}

void mymsgpop(queue *myqueue) {
  if (NULL == myqueue || 0 == myqueue -> size) {
    return;
  }

  if (1 == myqueue -> size) {
    Node *firstNode = myqueue -> firstNode;
    free(firstNode -> text);
    free(firstNode);
    --myqueue -> size;
    myqueue -> firstNode = NULL;
    myqueue -> lastNode = NULL;
    return;
  }

  Node *firstNode = myqueue -> firstNode;
  Node *secondNode = firstNode -> prev;

  free(firstNode -> text);
  free(firstNode);

  myqueue -> firstNode = secondNode;
  --myqueue -> size;
}

void mymsgdestroy(queue *myqueue) {
    mymsgdrop(myqueue);

    while(myqueue -> size > 0) {
      mymsgpop(myqueue);
    }

    sem_destroy(&myqueue -> sem);
    sem_destroy(&myqueue -> freeSem);
    sem_destroy(&myqueue -> busySem);
}

int mymsgget(queue *myqueue, char *buf, size_t bufsize) {
  if (myqueue -> isDestroyed) {
    return 0;
  }

  sem_wait(&myqueue -> busySem);
  sem_wait(&myqueue -> sem);

  if (myqueue -> isDestroyed) {
    sem_post(&myqueue -> busySem);
    sem_post(&myqueue -> freeSem);
    return 0;
  }

  Node *firstNode = myqueue -> firstNode;
  Node *secondNode = firstNode -> prev;

  strncpy(buf, firstNode -> text, bufsize - 1);
  buf[bufsize - 1] = '\0';


  free(firstNode -> text);
  free(firstNode);

  myqueue -> firstNode = secondNode;
  --myqueue -> size;

  if (0 == myqueue -> size) {
    myqueue -> lastNode = NULL;
  }

  sem_post(&myqueue -> sem);
  sem_post(&myqueue -> freeSem);


  return strlen(buf) + 1;
}

int mymsgput(queue *myqueue, char *text) {
  if (myqueue -> isDestroyed) {
    return 0;
  }

  sem_wait(&myqueue -> freeSem);
  sem_wait(&myqueue -> sem);

  if (myqueue -> isDestroyed) {
    sem_post(&myqueue -> freeSem);
    sem_post(&myqueue -> sem);
    return 0;
  }

  int length = strlen(text);

  if (length > MAX_LENGTH_OF_STRING) {
    length = MAX_LENGTH_OF_STRING;
  }

  Node *newNode = (Node*) malloc(sizeof(Node));
  newNode -> text = (char*) malloc (sizeof(char) * MAX_LENGTH_OF_STRING);

  strncpy(newNode -> text, text, length);
  newNode -> text[length] = '\0';

  if (myqueue -> size > 0) {
    myqueue -> lastNode -> prev = newNode;
    newNode -> next = myqueue -> lastNode;
    newNode -> prev = NULL;
    myqueue -> lastNode = newNode;
  } else {
    myqueue -> lastNode = newNode;
    myqueue -> firstNode = newNode;
  }

  ++myqueue -> size;

  sem_post(&myqueue -> sem);
  sem_post(&myqueue -> busySem);

  return length + 1;
}
