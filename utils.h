
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
#include <cstdlib>
#include <assert.h>
#include <cstring>
#include "zlib.h"

#define BINARY(x)    (print_binary(sizeof(x), &x))

typedef struct ultimate_image_file {
  FILE * pFile;
  unsigned lSize;
  unsigned char * buffer;
  size_t result;
} uFile;

void print_bit(long i, unsigned char * buffer);
long print_bits(long i, long j, unsigned char * buffer);
long print_bits_testrun(long i, long j, unsigned char * buffer);
long get_bits(long i, long j, unsigned char * buffer);
unsigned long long get_ll_bits(long i, long j, unsigned char * buffer);
unsigned char* get_bits_hex(long i, long j, unsigned char * buffer);
void print_buffer(long lSize, unsigned char * buffer);
uFile* open_file(const char* filename);
void close_file(uFile* image);
char* compress_string(char* unComp, unsigned len);
char* decompress_string(char* compD, unsigned len);
void print_binary(size_t const size, void const * const ptr);

#endif /* UTILS_H */

