#include <stdio.h>

#define LENREG 288 /* Lenght of register */
#define POS1 93
#define POS2 84
#define POS3 111

#define LENK 80
#define LENIV 80

#define LENSTREAM 300

#define NUMINIT 1152

struct Reg {
  _Bool data[LENREG];
  int pos;
} reg;

#define InitReg(rr) rr.pos = 0
#define Take(rr, pp) rr.data[(rr.pos + pp) % LENREG]
#define ShiftRightReg(rr) rr.pos = rr.pos == 0 ? LENREG-1 : (rr.pos-1) % LENREG
 

_Bool key[LENK], iv[LENIV];
_Bool stream[LENSTREAM];

void Shift(void) {
  _Bool t1, t2, t3;
  int ind;

  t1 = Take(reg,65)^Take(reg,90)&Take(reg,91)^Take(reg,92)^Take(reg,170);
  t2 = Take(reg,161)^Take(reg,174)&Take(reg,175)^Take(reg,176)^Take(reg,263);
  t3 = Take(reg,242)^Take(reg,285)&Take(reg,286)^Take(reg,287)^Take(reg,68);

  ShiftRightReg(reg);
  Take(reg, 0) = t3;
  Take(reg, POS1) = t1;
  Take(reg, POS1 + POS2) = t2;
}

int main() {
  int ind;
  _Bool t1, t2;
  int c, i;

  InitReg(reg);
  i = 0;
  while ((c = getchar()) != EOF) {
    if (c != '\n') {
      Take(reg, i) = c - '0';
      i++;
    }
    else {
      for (ind = 0; ind < LENSTREAM; ind++) {
        t1 = Take(reg,65) ^ Take(reg,92) ^ Take(reg,161);
        t2 = Take(reg,176) ^ Take(reg,242) ^ Take(reg,287);

        stream[ind] = t1 ^ t2;

        Shift();
      }
      for (ind = 0; ind < LENSTREAM; ind++)
        putchar(stream[ind] + '0');
      putchar('\n');

      InitReg(reg);
      i = 0;
    }
  }

  return 0;
}

