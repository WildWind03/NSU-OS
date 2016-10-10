#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LENGTH_OF_STRING 80
#define ALLOCATE_ERROR -1

typedef struct node {
  node *next;
  char *text;
} Node;

typedef struct list {
  node *lastNode;
  pthread_mutex_t mutex;
  size_t size;
} List;

int initList(List *list) {
  list = (List*) malloc (sizeof(List));
  list -> size = 0;
  return pthread_mutex_init(&list -> mutex, NULL);
}

int destroyList(List *list) {
  for (int k = 0; k < size; ++k) {

  }
  
  return pthread_mutex_destroy(&list -> mutex);
}

int add_(List *list, char *text, int start, int end){
  Node *node = (Node*) malloc (sizeof(Node));
  if (NULL == node) {
    return ALLOCATE_ERROR;
  }

  int length = strlen(text);
  if (end > length) {
    end = length;
  }

  node -> text = (char*) malloc (sizeof(char) * (end - start));
  strncpy(node -> text, text + start, end - start);

  node -> next = list -> lastNode;
  list -> lastNode = node;

  return 0;
}

int add(List *list, char *text) {
  pthread_mutex_lock(&list -> mutex);

  int length = strlen(text);
  int result = 0;

  for (int k = 0; k < length; k+=MAX_LENGTH_OF_STRING) {
    result = add_(list, text, k, k + MAX_LENGTH_OF_STRING);
    if (result < 0) {
      break;
    }
  }

  pthread_mutex_unlock(&list -> mutex);

  return result;
}

int bubbleSort(List *list) {
  pthread_mutex_lock(&list -> mutex);

  for (int k = 0; k < list -> size - 1; ++k) {
    bool swapped = false;
    Node *leftNode = list -> lastNode;

    for (int i = 0; i < list -> size - 1 - k; ++i) {
      Node *rightNode = leftNode -> next;

      if ((leftNode -> text)[0] > (rightNode -> text)[0]) {
        char *bufferText = rightNode -> text;
        rightNode -> text = leftNode -> text;
        leftNode -> text = bufferText;

        swapped = true;
      }

      leftNode = rightNode;
    }

    if (!swapped) {
      break;
    }
  }

  pthread_mutex_unlock(&list -> mutex);
}
