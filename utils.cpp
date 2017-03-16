
/* 
 * File:   utils.cpp
 * Author: gocmenta
 *
 * Created on March 2, 2017, 1:26 AM
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#include "utils.h"

using namespace std;

// TODO: multiplication can be optimized with shifts

void abort_utils(const char * s, ...) {
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

void print_bit(long i, unsigned char * buffer) {
  printf("buffer[%6ld]:   %02X\n", i, buffer[i]);
}

long print_bits(long i, long j, unsigned char * buffer) {
  long sum = 0;
  //  printf("buffer[%6d-%6d]:   ", i, j);

  if (j > i) {
    for (long k = i; k < j; k++) {
      printf("%02X", buffer[k]);
      sum *= 256;
      sum += (long) buffer[k];
    }
  } else {
    for (long k = i - 1; k > j - 1; k--) {
      printf("%02X", buffer[k]);
      sum *= 256;
      sum += (long) buffer[k];
    }
  }

  //  printf("            sum: = %lu\n", sum);
//  printf("\n");
  return sum;
}

long print_bits_testrun(long i, long j, unsigned char * buffer){
  long sum = 0;
  if (j > i) {
    for (long k = i; k < j; k++) {
      printf("%02X ", buffer[k]);
      sum *= 256;
      sum += (long) buffer[k];
    }
  } else {
    for (long k = i - 1; k > j - 1; k--) {
      printf("%02X ", buffer[k]);
      sum *= 256;
      sum += (long) buffer[k];
    }
  }
  return sum;
}

long get_bits(long i, long j, unsigned char * buffer) {
  long sum = 0;
  if (j > i) {
    for (long k = i; k < j; k++) {
      sum *= 256;
      sum += (long) buffer[k];
    }
  } else {
    for (long k = i - 1; k > j - 1; k--) {
      sum *= 256;
      sum += (long) buffer[k];
    }
  }
  return sum;
}

unsigned long long get_ll_bits(long i, long j, unsigned char * buffer) {
  unsigned long long sum = 0;
  if (j > i) {
    for (long k = i; k < j; k++) {
      sum *= 256;
      sum += (unsigned long long) buffer[k];
    }
  } else {
    for (long k = i - 1; k > j - 1; k--) {
      sum *= 256;
      sum += (unsigned long long) buffer[k];
    }
  }
  return sum;
}

unsigned char* get_bits_hex(long i, long j, unsigned char * buffer) {

  unsigned char* result;

  if (j > i) {
    result = new unsigned char[j - i];

    for (long k = i, l = 0; k < j; k++, l++) {
      result[l] = buffer[k];
    }
  } else if (j < i) {
    result = new unsigned char[i - j];

    for (long k = i - 1, l = 0; k > j - 1; k--, l++) {
      result[l] += buffer[k];
    }
  } else
    assert(0);

  return result;
}

void print_buffer(long lSize, unsigned char * buffer) {
  for (long i = 0; i < lSize; i += SCREEN_FACTOR) {
    for (long j = i; j < i + SCREEN_FACTOR && j < lSize; j++){
      print_bit(j, buffer);
    }
    char g;
    cin >> g;
  }
  return;
}

char* compress_string(char* unComp, unsigned len){
  
  uLong ucompSize = len+1;
  uLong compSize = compressBound(ucompSize);

  char* compD;
  compD = new char[compSize];
  
  // Deflate
  compress((Bytef *)compD, &compSize, (Bytef *)unComp, ucompSize);

  return compD;
}

char* decompress_string(char* compD, unsigned len){
  
  uLong ucompSize = len+1;
  uLong compSize = compressBound(ucompSize);

  char* unComp;
  unComp = new char[len];
  
  // Deflate
  uncompress((Bytef *)unComp, &ucompSize, (Bytef *)compD, compSize);
  
  return unComp;
}


uFile* open_file(const char* filename) {

  uFile* opened;
  opened = new uFile;

  opened->pFile = fopen(filename, "rb");
  if (opened->pFile == NULL) {
    fputs("File error", stderr);
    exit(1);
  }
  // obtain file size:
  fseek(opened->pFile, 0, SEEK_END);
  opened->lSize = ftell(opened->pFile);
  rewind(opened->pFile);

  // allocate memory to contain the whole file:
  opened->buffer = (unsigned char*) malloc(sizeof (unsigned char) * opened->lSize);
  if (opened->buffer == NULL) {
    fputs("Memory error", stderr);
    exit(2);
  }
  // copy the file into the buffer:
  opened->result = fread(opened->buffer, 1, opened->lSize, opened->pFile);
  if (opened->result != opened->lSize) {
    fputs("Reading error", stderr);
    exit(3);
  }

  return opened;
}

void close_file(uFile* image) {

  fclose(image->pFile);
  free(image->buffer);

  return;
}

