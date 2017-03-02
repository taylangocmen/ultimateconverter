
/* 
 * File:   bmp.cpp
 * Author: gocmenta
 *
 * Created on March 2, 2017, 1:20 AM
 */

#include <iostream>
#include <assert.h>

#include "utils.h"
#include "bmp.h"

using namespace std;

//void print_bmp(long lSize, unsigned char * buffer){

void print_bmp(uImage* img) {
  cout << "Going to print BMP" << endl << endl;

  unsigned char* buffer = img->buffer;
  long lSize = img->lSize;

  cout << "BMP Header-----------------------------------" << endl;
  long signature = get_bits(0, 2, buffer);
  cout << "signature: " << signature << endl << endl;

  long fileSize = get_bits(6, 2, buffer);
  cout << "fileSize: " << fileSize << endl << endl;

  long reserved1 = get_bits(6, 8, buffer);
  cout << "reserved1: " << reserved1 << endl << endl;

  long reserved2 = get_bits(8, 10, buffer);
  cout << "reserved2: " << reserved2 << endl << endl;

  long offsetPixelArray = get_bits(14, 10, buffer);
  cout << "offsetPixelArray: " << offsetPixelArray << endl << endl;


  cout << "DIB Header-----------------------------------" << endl;
  long dibHeaderSize = get_bits(18, 14, buffer);
  cout << "dibHeaderSize: " << dibHeaderSize << endl << endl;

  long imageWidth = get_bits(22, 18, buffer);
  cout << "imageWidth: " << imageWidth << endl << endl;

  long imageHeight = get_bits(26, 22, buffer);
  cout << "imageHeight: " << imageHeight << endl << endl;

  long planes = get_bits(28, 26, buffer); /// Planes
  cout << "planes: " << planes << endl << endl;

  long bitsPerPixel = get_bits(30, 28, buffer);
  cout << "bitsPerPixel: " << bitsPerPixel << endl << endl;

  long compression = get_bits(34, 30, buffer);
  cout << "compression: " << compression << endl << endl;

  long imageSize = get_bits(38, 34, buffer);
  cout << "imageSize: " << imageSize << endl << endl;


  cout << "Headers finished------------------------------" << endl;
  long endPixelArray = offsetPixelArray + imageSize;
  cout << "endPixelArray: " << endPixelArray << endl << endl;


  cout << "Pixel array parameters------------------------" << endl;
  long bytesPerPixel = bitsPerPixel / 8;
  cout << "bytesPerPixel: " << bytesPerPixel << endl;

  long lineSize = imageSize / imageHeight;
  cout << "lineSize: " << lineSize << endl;

  long paddingPerLine = lineSize % bitsPerPixel;
  cout << "paddingPerLine: " << paddingPerLine << endl;

  assert(lineSize - paddingPerLine == imageWidth * bytesPerPixel);

  cout << "Start the pixel array printing----------------" << endl;
  for (long i = offsetPixelArray; i < endPixelArray; i += lineSize) {

    for (long j = i; j < i + lineSize - paddingPerLine; j += bytesPerPixel) {
      print_bits(j, j + bytesPerPixel, buffer);
      cout << "  ";
    }
    char g;
    cin >> g;
//    cout << "g is: " << g << endl;
    const char gg = g;
    if(gg == 'q') {
      break;
    }
  }

  return;
}
