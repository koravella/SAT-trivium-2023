#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int c;
  int i;

  i = 0;
  if (strcmp(argv[1], "in") == 0) {
    while ((c = getchar()) != '\n') {
      printf("%s%d%s%d%s\n", "__CPROVER_assume(Take(reg, ", i, ") == ", c - '0', ");");
      i++;
    }
  }
  else if (strcmp(argv[1], "out") == 0) {
    while ((c = getchar()) != '\n') {
      printf("%s%d%s%d%s\n", "__CPROVER_assume(stream[", i, "] == ", c - '0', ");");
      i++;
    }
  }
  return 0;
}
