/* A CBMC program for the Trivium stream cipher without initialization stage */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int reg_len = 288;
  int lenA = 93;
  int lenB = 84;
  int lenC = 111; 
  int stream_len = 300;
  _Bool reg[reg_len];
  _Bool stream[stream_len];
  _Bool t1;
  _Bool t2;
  _Bool t3;
  int i;
  int j;
  FILE *in, *out;
  int tmp;
  
  if ((in = fopen(argv[1], "r")) == NULL)
    exit(EXIT_FAILURE);
  for (int k = 0; k < reg_len; k++) {
    if (fscanf(in, "%d\n", &tmp) == 0)
      exit(EXIT_FAILURE);
    else
      reg[k] = tmp;
  }
  fclose(in);

  /* Performing LENSTREAM steps */ 
  for(i = 0; i < stream_len; i = i + 1) {
    stream[i] = reg[65]^reg[92]^reg[161]^reg[176]^reg[242]^reg[287];
	  t1 = reg[65]^reg[90]&reg[91]^reg[92]^reg[170];
	  t2 = reg[161]^reg[174]&reg[175]^reg[176]^reg[263];
	  t3 = reg[242]^reg[285]&reg[286]^reg[287]^reg[68];

	  for(j = lenA - 1; j > 0; j = j - 1)
	    reg[j] = reg[j-1];
	  reg[0] = t3;

	  for(j = lenA + lenB - 1; j > lenA; j = j - 1)
	    reg[j] = reg[j-1];
	  reg[lenA] = t1;

	  for(j = lenA + lenB + lenC - 1; j > lenA + lenB; j = j - 1)
	    reg[j] = reg[j-1];
	  reg[lenA + lenB] = t2;
  }

  if ((out = fopen(argv[2], "w")) == NULL)
    exit(EXIT_FAILURE);
  for (int k = 0; k < stream_len; k++) {
    if (fprintf(in, "%d\n", stream[k]) < 0)
      exit(EXIT_FAILURE);
  }
  fclose(out);

  return 0;
}
