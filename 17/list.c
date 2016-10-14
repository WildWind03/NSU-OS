#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LENGTH_OF_STRING 80
#define ALLOCATE_ERROR -1
#define EMPTY_LIST_ERROR -2

typedef struct node {
  pthread_mutex_t mutex;
  node *next;
  char *text;
} Node;

typedef struct list {
  pthread_mutex_t mutex;
  node *lastNode;
  size_t size;
} List;

List* createList() {
  List* list = (List*) malloc (sizeof(List));
  list -> size = 0;
  pthread_mutex_init(&list -> mutex, NULL);
  return list;
}

int deleteLast(List *list) {
  pthread_mutex_lock(list -> mutex);

  if (list -> size > 0) {
    Node *lastNode = list -> lastNode;
    Node *newLastNode = lastNode -> next;

    free (lastNode -> text);
    pthread_mutex_destroy(&lastNode -> mutex);
    free (lastNode);

    list -> lastNode = newLastNode;
    list -> size--;
    pthread_mutex_unlock(&list -> mutex);
    return 0;
  } else {
    pthread_mutex_unlock(&list -> mutex);
    return EMPTY_LIST_ERROR;
  }
}


void destroyList(List *list) {
  while (EMPTY_LIST_ERROR != deleteLast(list)) {
  }

  pthread_mutex_destroy(&list -> mutex);

  free(list);
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

  pthread_mutex_init(&node -> mutex, NULL);
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
  if (0 == list -> size) {
    return 0;
  }

  Node *leftNode;
  for (int k = 0; k < list -> size - 1; ++k) {
    bool swapped = false;
    pthread_mutex_lock(&list -> mutex);

    pthread_mutex_lock(&list -> lastNode -> mutex);
    pthread_mutex_lock(&list -> lastNode -> next -> mutex);

    leftNode = list -> lastNode;

    for (int i = 0; i < list -> size - 1 - k; ++i) { //TODO dynamic size of the list, RIGHT work with mutexes
      if (1 == i) {
        pthread_mutex_unlock(&list -> mutex);
      }

      Node *rightNode = leftNode -> next;

      if (isLeftBigger(leftNode -> text, rightNode -> text)) {
        char *bufferText = rightNode -> text;
        rightNode -> text = leftNode -> text;
        leftNode -> text = bufferText;

        swapped = true;
      }

      Node *oldLeftNode = leftNode;
      leftNode = rightNode;

      pthread_mutex_unlock(&oddLeftNode -> mutex);
      pthread_mutex_unlock(&leftNode -> mutex);

      pthread_mutex_lock(&leftNode -> mutex);
      pthread_mutex_lock(&leftNode -> next -> mutex);
    }

    pthread_mutex_unlock(&leftNode -> mutex);
    pthread_mutex_unlock(&leftNode -> next -> mutex);

    if (!swapped) {
      break;
    }
  }
}
