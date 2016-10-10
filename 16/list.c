#define MAX_LENGTH_OF_STRING 80
#define ALLOCATE_ERROR -1

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct node {
  node *next;
  char *text;
} Node;

typedef struct list {
  node *lastNode;
  pthread_mutex_t *mutex;
  size_t size;
} List;

int initList(List *list) {
  list -> size = 0;
  return pthread_mutex_init(list -> mutex, NULL);
}

int destroyList(List *list) {
  return pthread_mutex_destroy(list -> mutex);
}

int add(List *list, char *text) {
  pthread_mutex_lock(list -> mutex);

  Node *node = (Node*) malloc (sizeof(Node));
  if (NULL == node) {
    return ALLOCATE_ERROR;
  }

  int length = strlen(text);
  node -> text = (char*) malloc (sizeof(char) * length);
  strcpy(node -> text, text);

  node -> next = list -> lastNode;
  list -> lastNode = node;

  pthread_mutex_unlock(list -> mutex);

  return 0;
}
