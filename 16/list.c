#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LENGTH_OF_STRING 80
#define ALLOCATE_ERROR -1
#define EMPTY_LIST_ERROR -2

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
  list -> size = 0;
  return pthread_mutex_init(&list -> mutex, NULL);
}

int deleteLast(List *list) {
  if (list -> size > 0) {
    Node *lastNode = list -> lastNode;
    Node *newLastNode = lastNode -> next;

    free (lastNode -> text);
    free (lastNode);

    list -> lastNode = newLastNode;
    list -> size--;
    return 0;
  } else {
    return EMPTY_LIST_ERROR;
  }
}


int destroyList(List *list) {
  while (EMPTY_LIST_ERROR != deleteLast(list)) {
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

  node -> text = (char*) malloc (sizeof(char) * (end - start + 1));
  strncpy(node -> text, text + start, end - start);
  (node -> text)[end - start] = '\0';

  node -> next = list -> lastNode;
  list -> lastNode = node;
  list -> size++;

  return 0;
}

int addToList(List *list, char *text) {
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

bool isLeftBigger(char *textLeft, char *textRight) {
    int leftLength = strlen(textLeft);
    int rightLength = strlen(textRight);

    int minLength = leftLength > rightLength ? rightLength : leftLength;

    for (int k = 0; k < minLength; ++k) {
      if (textLeft[k] > textRight[k]) {
        return true;
      } else {
        if (textLeft[k] < textRight[k]) {
          return false;
        }
      }
    }

    if (leftLength > rightLength) {
      return true;
    }

    return false;
}

int bubbleSort(List *list) {
  pthread_mutex_lock(&list -> mutex);

  for (int k = 0; k < list -> size - 1; ++k) {
    bool swapped = false;
    Node *leftNode = list -> lastNode;

    for (int i = 0; i < list -> size - 1 - k; ++i) {
      Node *rightNode = leftNode -> next;

      if (isLeftBigger(leftNode -> text, rightNode -> text)) {
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
