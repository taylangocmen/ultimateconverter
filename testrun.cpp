
/* 
 * File:   testrun.cpp
 * Author: gocmenta
 *
 * Created on March 9, 2017, 7:07 PM
 */

#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <png.h>

#include "utils.h"
#include "pngsample.h"
#include "uc_bmp.h"

#define PNG_DEBUG 3

// originally
//#define MASK_RED(p)    (((p) & 0xE0) >> 5)
//#define MASK_GREEN(p)    (((p) & 0x1A) >> 2)
//#define MASK_BLUE(p)    (((p) & 0x3) >> 0)
// test
#define MASK_BLUE(p)    (((p) & 0xC0) >> 6)
#define MASK_GREEN(p)    (((p) & 0x38) >> 3)
#define MASK_RED(p)    (((p) & 0x07) >> 0) 


using namespace std;

void tr_abort_(const char * s, ...) {
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

png_structp tr_png_ptr;
png_infop tr_info_ptr;
int tr_width, tr_height;

png_byte tr_color_type = '\x02';
png_byte tr_bit_depth= '\x08';
png_bytep * tr_row_pointers;

void tr_write_png_file(char* file_name) {
  FILE *fp = fopen(file_name, "wb");
  if (!fp)
    tr_abort_("[tr_write_png_file] File %s could not be opened for writing", file_name);
  tr_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  
  if (!tr_png_ptr)
    tr_abort_("[tr_write_png_file] png_create_write_struct failed");
  tr_info_ptr = png_create_info_struct(tr_png_ptr);
  
  if (!tr_info_ptr)
    tr_abort_("[tr_write_png_file] png_create_info_struct failed");
  
  if (setjmp(png_jmpbuf(tr_png_ptr)))
    tr_abort_("[tr_write_png_file] Error during init_io");
  png_init_io(tr_png_ptr, fp);
  
  if (setjmp(png_jmpbuf(tr_png_ptr)))
    tr_abort_("[tr_write_png_file] Error during writing header");
  
  png_set_IHDR(tr_png_ptr, tr_info_ptr, tr_width, tr_height,
    tr_bit_depth, tr_color_type, PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  
  png_write_info(tr_png_ptr, tr_info_ptr);
  
  if (setjmp(png_jmpbuf(tr_png_ptr)))
    tr_abort_("[tr_write_png_file] Error during writing bytes");
  png_write_image(tr_png_ptr, tr_row_pointers);
  
  if (setjmp(png_jmpbuf(tr_png_ptr)))
    tr_abort_("[tr_write_png_file] Error during end of write");
  png_write_end(tr_png_ptr, NULL);
  
  fclose(fp);
}

png_bytep * testrun1(void){
  const char* bmpName = "bmp_testrun1.bmp";
//  const char* bmpName = "bmp_solidblack.bmp";
//  const char* bmpName = "bmp_solidwhite.bmp";
  
  uImage* bmpImage = open_file(bmpName);
  unsigned char* buffer = bmpImage->buffer;

  long lSize = bmpImage->lSize;
  
  cout << "BMP Header-----------------------------------" << endl;
  long signature = get_bits(0, 2, buffer);
  cout << "signature: " << signature << endl;

  long fileSize = get_bits(6, 2, buffer);
  cout << "fileSize: " << fileSize << endl;

  long reserved1 = get_bits(6, 8, buffer);
  cout << "reserved1: " << reserved1 << endl;

  long reserved2 = get_bits(8, 10, buffer);
  cout << "reserved2: " << reserved2 << endl;

  long offsetPixelArray = get_bits(14, 10, buffer);
  cout << "offsetPixelArray: " << offsetPixelArray << endl;


  cout << endl << "DIB Header-----------------------------------" << endl;
  long dibHeaderSize = get_bits(18, 14, buffer);
  cout << "dibHeaderSize: " << dibHeaderSize << endl;

  long imageWidth = get_bits(22, 18, buffer);
  cout << "imageWidth: " << imageWidth << endl;

  long imageHeight = get_bits(26, 22, buffer);
  cout << "imageHeight: " << imageHeight << endl;

  long planes = get_bits(28, 26, buffer); /// Planes
  cout << "planes: " << planes << endl;

  long bitsPerPixel = get_bits(30, 28, buffer);
  cout << "bitsPerPixel: " << bitsPerPixel << endl;

  long compression = get_bits(34, 30, buffer);
  cout << "compression: " << compression << endl;

  long imageSize = get_bits(38, 34, buffer);
  cout << "imageSize: " << imageSize << endl;

  long xPixelsPM = get_bits(42, 38, buffer);
  cout << "xPixelsPM: " << xPixelsPM << endl;
  
  long yPixelsPM = get_bits(46, 42, buffer);
  cout << "yPixelsPM: " << yPixelsPM << endl;
  
  long colorsInTable = get_bits(50, 46, buffer);
  cout << "colorsInTable: " << colorsInTable << endl;
  
  long importantColorC = get_bits(54, 50, buffer);
  cout << "importantColorC: " << importantColorC << endl;
  
  long** colorTable;
  colorTable = new long*[colorsInTable];
  for(int i = 0; i < colorsInTable; i++)
    colorTable[i] = new long[3];
  
  
  cout << endl << "Color Table------------------------------" << endl;
  
  unsigned j = 0;
  for(long index = BMP_HEADER_SIZE + dibHeaderSize; 
    index < BMP_HEADER_SIZE + dibHeaderSize + (4 * colorsInTable); 
    index+=4, j++){
    
    long currColor = get_bits(index+4, index, buffer);
//    cout << "currColor " << currColor << endl;
//    cout << "color: ";
//    print_bits(index+4, index, buffer);
//    cout << endl;
    
//    cout << "red: ";
    colorTable[j][0] = get_bits(index+3, index+2, buffer);
//    cout << endl;
    
//    cout << "green: ";
    colorTable[j][1] = get_bits(index+2, index+1, buffer);
//    cout << endl;
    
//    cout << "blue: ";
    colorTable[j][2] = get_bits(index+1, index, buffer);
//    cout << endl << endl;
  }
  
  assert(j == colorsInTable);
  
//  long redChannelBitmask = get_bits(58, 54, buffer);
//  cout << endl << "redChannelBitmask: " << redChannelBitmask << endl;
//  BINARY(redChannelBitmask);
//  
//  long greenChannelBitmask = get_bits(62, 58, buffer);
//  cout << endl << "greenChannelBitmask: " << greenChannelBitmask << endl;
//  BINARY(greenChannelBitmask);
//  
//  long blueChannelBitmask = get_bits(66, 62, buffer);
//  cout << endl << "blueChannelBitmask: " << blueChannelBitmask << endl;
//  BINARY(blueChannelBitmask);
//  
//  long alphaChannelBitmask = get_bits(70, 66, buffer);
//  cout << endl << "alphaChannelBitmask: " << alphaChannelBitmask << endl;
//  BINARY(alphaChannelBitmask);
  
  
  cout << endl << "Headers finished------------------------------" << endl;
  long endPixelArray = offsetPixelArray + imageSize;
  cout << "endPixelArray: " << endPixelArray << endl;


  cout << endl << "Pixel array parameters------------------------" << endl;
  long bytesPerPixel = bitsPerPixel / 8;
  cout << "bytesPerPixel: " << bytesPerPixel << endl;

  long lineSize = imageSize / imageHeight;
  cout << "lineSize: " << lineSize << endl;

  long paddingPerLine = lineSize % bitsPerPixel;
  cout << "paddingPerLine: " << paddingPerLine << endl;

  assert(lineSize - paddingPerLine == imageWidth * bytesPerPixel);
  cout << "bytesPerPixel: " << bytesPerPixel << endl;
  
  tr_width = (int)imageWidth;
  tr_height = (int)imageHeight;
  
  long*** pixelArray;
  pixelArray = new long**[imageHeight];
  for(int i = 0; i < imageHeight; i++){
    
    pixelArray[i] = new long*[imageWidth];
    
    for(int j = 0; j < imageWidth; j++){
      
      pixelArray[i][j] = new long[3];
    }
  }
  
  int rowIndex = imageHeight - 1;
  for (long i = offsetPixelArray; i < endPixelArray; i += lineSize, rowIndex--) {
  
    int colIndex = 0;
    for (long j = i; j < i + lineSize - paddingPerLine; j += bytesPerPixel, colIndex++) {
      long pixel = get_bits(j, j + bytesPerPixel, buffer);

//      long red = MASK_RED(color);
//      long red256 = 255.0 * (red / 7.00);
////      long channelred = (color & redChannelBitmask);
//
//      long green = MASK_GREEN(color);
//      long green256 = 255.0 * (green / 7.00);
////      long channelgreen = (color & greenChannelBitmask);
//
//      long blue = MASK_BLUE(color);
//      long blue256 = 255.0 * (blue / 3.00);
////      long channelblue = (color & blueChannelBitmask);

      pixelArray[rowIndex][colIndex][0] = colorTable[pixel][0];
      pixelArray[rowIndex][colIndex][1] = colorTable[pixel][1];
      pixelArray[rowIndex][colIndex][2] = colorTable[pixel][2];


//      cout << "color " << color << endl;
//      BINARY(color);
//      
//      cout << "red: " << red;
//      cout << " - green: " << green;
//      cout << " - blue: " << blue << endl;
//
//      cout << "red256: " << red256;
//      cout << " - green256: " << green256;
//      cout << " - blue256: " << blue256 << endl;

//      cout << "channelred: " << channelred;
//      cout << " - channelgreen: " << channelgreen;
//      cout << " - channelblue: " << channelblue << endl << endl;
    }
    
    assert(colIndex == imageWidth);
  }
  
  assert(rowIndex == -1);
  
//  tr_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//  tr_info_ptr = png_create_info_struct(tr_png_ptr);
  
  tr_row_pointers = (png_bytep*) malloc(sizeof (png_bytep) * tr_height);
  for (int y = 0; y < tr_height; y++)
//    tr_row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(tr_png_ptr, tr_info_ptr));
    tr_row_pointers[y] = (png_byte*) malloc(3 * tr_width);
  
  for (int y = 0; y < tr_height; y++) {
    png_byte* row = tr_row_pointers[y];
    for (int x = 0; x < tr_width; x++) {
      png_byte* ptr = &(row[x * 3]);
//      printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d\n",
//        x, y, ptr[0], ptr[1], ptr[2]); if(x == tr_width-1) printf("\n");
      ptr[0] = pixelArray[y][x][0];
      ptr[1] = pixelArray[y][x][1];
      ptr[2] = pixelArray[y][x][2];
    }
  }

  char* outfile = (char*)"bmp_to_png_testrun1.png";
  tr_write_png_file(outfile);
  
  
  for(int i = 0; i < imageHeight; i++){
    for(int j = 0; j < imageWidth; j++)
      delete[] pixelArray[i][j];
    delete[] pixelArray[i];
  }
  delete[] pixelArray;
  
//  for (int y = 0; y < tr_height; y++)
//    free(tr_row_pointers[y]);
//  free(tr_row_pointers);
  
  
  close_file(bmpImage);
  free(bmpImage);
  
  cout << "test run ------------------" << endl;
  cout << "width " << tr_width << endl;
  cout << "height " << tr_height << endl;
  printf("color_type %02X\n", tr_color_type);
  printf("bit_depth %02X\n", tr_bit_depth);
  cout << "png_get_rowbytes(png_ptr, info_ptr) " 
    << png_get_rowbytes(tr_png_ptr, tr_info_ptr) << endl;
  cout << "sizeof (png_bytep) * height "
    << sizeof (png_bytep) * tr_height << endl;
  
  
  return tr_row_pointers;
}
