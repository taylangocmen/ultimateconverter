
/* 
 * File:   uUtils.cpp
 * Author: gocmenta
 *
 * Created on March 14, 2017, 1:32 AM
 */

#include <stdlib.h>
#include <stdio.h>

using namespace std;

unsigned to_power(unsigned base, unsigned power){
  if(power == 0 || base == 1)
    return 1;
  
  unsigned res = base;
  
  for(unsigned i = 1; i < power; i++)
    res *= base;
  
  return res;
}

void print_binary(size_t const size, void const * const ptr) {
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;

  for (i = size-1; i >= 0; i--) {
    for (j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }
  printf("\n");

  return;
}