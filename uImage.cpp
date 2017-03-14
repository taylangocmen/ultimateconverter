
/* 
 * File:   uImage.cpp
 * Author: gocmenta
 * 
 * Created on March 13, 2017, 10:24 PM
 */

#include "uImage.h"

uImage::uImage() {
}

uImage::uImage(const char* fileName) {

  file = fopen(fileName, "rb");
  if (file == NULL)
    abort_("Cannot open %s", fileName);

  fseek(file, 0, SEEK_END);
  fSize = ftell(file);
  rewind(file);

  fBuffer = new unsigned char[fSize];
  if (fBuffer == NULL)
    abort_("Memory error allocating for %s", fileName);

  size_t result = fread(fBuffer, 1, fSize, file);
  if (result != fSize)
    abort_("Cannot read %s", fileName);



}

uImage::uImage(const uImage& orig) {
}

uImage::~uImage() {

  delete_all();
}

void uImage::convert(format fmt) {

  check_status();

  //TODO: do the writebacks
}

void uImage::abort_(const char* s, ...) {
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);

  delete_all();
  abort();
}

void uImage::delete_all() {

  if (fBuffer != NULL) {
    delete fBuffer;
    fBuffer = NULL;
  }

  if (file != NULL) {
    delete file;
    file = NULL;
  }

  if (pxArr != NULL) {
    for (unsigned i = 0; i < pxHeight; i++) {
      for (unsigned j = 0; j < pxWidth; j++) {
        delete[] pxArr[i][j];
      }
      delete[] pxArr[i];
    }
    delete[] pxArr;
    pxArr = NULL;
  }

}

void uImage::check_status() {

  //TODO: fill this
}

void uImage::decipher_bmp() {

}

void uImage::decipher_jpg() {

}

void uImage::decipher_png() {

}

void uImage::write_bmp() {

}

void uImage::write_jpg() {

}

void uImage::write_png() {

}

