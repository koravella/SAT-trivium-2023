#include <stdio.h>

#define LENREG 288

void put0(int n) {
  for (int i = 0; i < n; i++)
    putchar('0' + 0);
}

void put1(int n) {
  for (int i = 0; i < n; i++)
    putchar('0' + 1);
}

int main(void) {
  put0(LENREG);
  putchar('\n');

  put1(LENREG);
  putchar('\n');

  put0(LENREG / 2);
  put1(LENREG / 2);
  putchar('\n');

  put1(LENREG / 2);
  put0(LENREG / 2);
  putchar('\n');

  return 0;
}
