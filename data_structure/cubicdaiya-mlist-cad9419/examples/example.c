#include <stdio.h>
#include <string.h>
#include <mlist.h>

int main (int argc, char *argv[]) {
  
  mlist_t *list;
  int  *n  = NULL;
  char *c  = NULL;
  char *s1 = NULL;
  char *s2 = NULL;
  const char *name[] = {"bokko", "cubicdaiya"};
  list = mlist_create();
  n    = mlist_alloc(&list, sizeof(int));
  c    = mlist_alloc(&list, sizeof(char));
  s1   = mlist_alloc(&list, strlen(name[0]) + 1);
  s2   = mlist_alloc(&list, strlen(name[1]) + 1);
  *n   = 5;
  *c   = 'a';
  strncpy(s1, name[0], strlen(name[0]) + 1);
  strncpy(s2, name[1], strlen(name[1]) + 1);
  printf("n  = %d\n", *n);
  printf("c  = %c\n", *c);
  printf("s1 = %s\n", s1);
  printf("s2 = %s\n", s2);
  mlist_destroy(list);
  
  return 0;
}
