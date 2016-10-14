#include "list.c"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LENGTH_OF_STRING_FOR_INPUT 1000

void* doIt(void *arg) {
  List* list = (List*) arg;

  while(1) {
    bubbleSort(list);
    sleep(5);
  }
}

bool isEmpty(char *text) {
  if (1 == strlen(text) && '\n' == text[0]) {
    return true;
  } else {
    return false;
  }
}

int main(int argc, char const *argv[]) {
  List *list = createList();

  pthread_t thread;

  int resultOfThreadCreation = pthread_create(&thread, NULL, doIt, list);

  char *text = (char*) malloc (sizeof(char) * MAX_LENGTH_OF_STRING_FOR_INPUT);

  while(1) {
    fgets(text, MAX_LENGTH_OF_STRING_FOR_INPUT, stdin);

    if (isEmpty(text)) {
      Node *node = list -> lastNode;
      printf ("START\n");
      for (int k = 0; k < list -> size; ++k) {
        printf ("%s\n", node -> text);
        node = node -> next;
      }
      printf ("END\n");
    } else {

      int k = strlen(text);
      text[k - 1] = '\0';

      addToList(list, text);
    }
  }

  pthread_join(thread, NULL);

  free(text);
  destroyList(list);
  return 0;
}
