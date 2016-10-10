#include "list.c"

int main(int argc, char const *argv[]) {
  List list;
  initList(&list);
  addToList(&list, "Hey");
  addToList(&list, "Hey2");
  addToList(&list, "Hay");
  addToList(&list, "Hay00");
  addToList(&list, "nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn");
  bubbleSort(&list);

  Node *node = list.lastNode;
  for (int k = 0; k < list.size; ++k) {
    printf ("%s\n", node -> text);
    node = node -> next;
  }
  destroyList(&list);
  return 0;
}
