
/* 
 * File:   uImage.h
 * Author: gocmenta
 *
 * Created on March 13, 2017, 10:24 PM
 */

#ifndef UIMAGE_H
#define UIMAGE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#include "uUtils.h"

using namespace std;

class uImage {
public:
  uImage();
  uImage(const char* fileName);
  uImage(const uImage& orig);
  virtual ~uImage();
  void convert(format fmt);

private:
  unsigned short*** pxArr;
  unsigned pxHeight;
  unsigned pxWidth;
  unsigned pxFormat;

  FILE* file;
  unsigned char* fBuffer;
  size_t fSize;
  format fFormat;

  void abort_(const char* s, ...);
  void delete_all();
  void check_status();

  void decipher_bmp();
  void decipher_jpg();
  void decipher_png();

  void write_bmp();
  void write_jpg();
  void write_png();

};

#endif /* UIMAGE_H */

