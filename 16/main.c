#include "list.c"

int main(int argc, char const *argv[]) {
  List list;
  initList(&list);
  destroyList(&list);
  return 0;
}
