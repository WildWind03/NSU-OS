#include "list.c"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

int main() {
  queue queue;
  mymsginit(&queue);
  mymsgput(&queue, "Hello");
  mymsgput(&queue, "Hello1");
  mymsgput(&queue, "Hello2");
  mymsgput(&queue, "Hello3");

  char buffer[20];

  mymsgget(&queue, buffer, 20);
  printf ("%s\n", buffer);

  mymsgget(&queue, buffer, 20);
  printf ("%s\n", buffer);

  mymsgget(&queue, buffer, 20);
  printf ("%s\n", buffer);

  mymsgget(&queue, buffer, 20);
  printf ("%s\n", buffer);
  mymsgdestroy(&queue);
  return 0;
}
