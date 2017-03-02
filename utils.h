
/* 
 * File:   utils.h
 * Author: gocmenta
 *
 * Created on March 2, 2017, 1:22 AM
 */

#ifndef UTILS_H
#define UTILS_H

#define SCREEN_FACTOR 65

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

typedef struct ultimate_image {
  FILE * pFile;
  long lSize;
  unsigned char * buffer;
  size_t result;
} uImage;

void print_bit(long i, unsigned char * buffer);
long print_bits(long i, long j, unsigned char * buffer);
long get_bits(long i, long j, unsigned char * buffer);
unsigned long long get_ll_bits(long i, long j, unsigned char * buffer);
unsigned char* get_bits_hex(long i, long j, unsigned char * buffer);
void print_buffer(long lSize, unsigned char * buffer);
uImage* open_file(const char* filename);
void close_file(uImage* image);

#endif /* UTILS_H */

