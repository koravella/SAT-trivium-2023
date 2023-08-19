/* A CBMC program for the Trivium stream cipher without initialization stage */

#include <stdio.h>

void main()
{
  int reg_len = 288;
  int lenA = 93;
  int lenB = 84;
  int lenC = 111; 
  int stream_len = 300;
  __CPROVER_bool input[reg_len];
  __CPROVER_bool reg[reg_len];
  __CPROVER_bool stream[stream_len];
  __CPROVER_bool t1;
  __CPROVER_bool t2;
  __CPROVER_bool t3;
  int i;
  int j;

  /* Setting assume for all input bits (or some part of the bits) */
/*
  for (i = 0; i < reg_len; i++)
    __CPROVER_assume(input[i] == 0);
*/
  for (i = 0; i < reg_len; i++)
    reg[i] = input[i];

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

   /* Setting assume for all stream bits (or some part of the bits) */
/*  
  for (i = 0; i < stream_len; i++)
    __CPROVER_assume(stream[i] == 0);
*/

  /* To generate the cbmc property */
  __CPROVER_assert(0,"test");
}
