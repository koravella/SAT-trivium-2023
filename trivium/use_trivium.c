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

void Init(void) {
  int ind;

  InitReg(reg);
  for (ind = 0; ind < LENK; ind++)
    Take(reg, ind) = key[ind];
  for (ind = LENK; ind < POS1; ind++)
    Take(reg, ind) = 0;
  for (ind = POS1; ind < POS1 + LENIV; ind++)
    Take(reg, ind) = iv[ind - POS1];
  for (ind = POS1 + LENIV; ind < LENREG - 3; ind++);
    Take(reg, ind) = 0;
  Take(reg, LENREG - 3) = 1;
  Take(reg, LENREG - 2) = 1;
  Take(reg, LENREG - 1) = 1;

  for (ind = 0; ind < NUMINIT; ind++)
    Shift();
}

void in(void) {
  int i;

  for(i = 0; i < LENK; i++)
    key[i] = 0;  
  for(i = 0; i < LENIV; i++)
    iv[i] = 0;
}

void out(void) {
  int i;

  for (i = 0; i < LENSTREAM; i++)
    printf("%d", stream[i]);
  puts("");
}

int main() {
  int ind;
  _Bool t1, t2;

  in();

  Init();

  for (ind = 0; ind < LENSTREAM; ind++) {
    t1 = Take(reg,65) ^ Take(reg,92) ^ Take(reg,161);
    t2 = Take(reg,176) ^ Take(reg,242) ^ Take(reg,287);

    stream[ind] = t1 ^ t2;

    Shift();
  }

  out();

  return 0;
}
