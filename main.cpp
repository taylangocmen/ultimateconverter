#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <assert.h>

#include "headers.h"

using namespace std;

// TODO: multiplication can be optimized with 


void printBIT(long i, unsigned char * buffer);
long printBITS(long i, long j, unsigned char * buffer);
void printbuffer(long lSize, unsigned char * buffer);
void printJPG(long lSize, unsigned char * buffer);
void printPNG(long lSize, unsigned char * buffer);
void printBMP(long lSize, unsigned char * buffer);

int main() {
  FILE * pFile;
  long lSize;
  unsigned char * buffer;
  size_t result;
  
  FILE * pFile2;
  long lSize2;
  unsigned char * buffer2;
  size_t result2;
  
  FILE * pFile3;
  long lSize3;
  unsigned char * buffer3;
  size_t result3;
  
  /****************************************************************************/
  /***************************                      ***************************/
  /*********************               JPG                *********************/
  /***************************                      ***************************/
  /****************************************************************************/
  {
    pFile = fopen("originalJPG.jpg", "rb");
    if (pFile == NULL) {
      fputs("File error", stderr);
      exit(1);
    }
    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (unsigned char*) malloc(sizeof (unsigned char)*lSize);
    if (buffer == NULL) {
      fputs("Memory error", stderr);
      exit(2);
    }
    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize) {
      fputs("Reading error", stderr);
      exit(3);
    }
  }
  
  /****************************************************************************/
  /***************************                      ***************************/
  /*********************               PNG                *********************/
  /***************************                      ***************************/
  /****************************************************************************/
  {
    pFile2 = fopen("originalPNG.png", "rb");
    if (pFile2 == NULL) {
      fputs("File error", stderr);
      exit(1);
    }
    // obtain file size:
    fseek(pFile2, 0, SEEK_END);
    lSize2 = ftell(pFile2);
    rewind(pFile2);

    // allocate memory to contain the whole file:
    buffer2 = (unsigned char*) malloc(sizeof (unsigned char)*lSize2);
    if (buffer2 == NULL) {
      fputs("Memory error", stderr);
      exit(2);
    }
    // copy the file into the buffer2:
    result2 = fread(buffer2, 1, lSize2, pFile2);
    if (result2 != lSize2) {
      fputs("Reading error", stderr);
      exit(3);
    }
  }
  
  /****************************************************************************/
  /***************************                      ***************************/
  /*********************               BMP                *********************/
  /***************************                      ***************************/
  /****************************************************************************/
  {
    pFile3 = fopen("originalBMP.bmp", "rb");
    if (pFile3 == NULL) {
      fputs("File error", stderr);
      exit(1);
    }
    // obtain file size:
    fseek(pFile3, 0, SEEK_END);
    lSize3 = ftell(pFile3);
    rewind(pFile3);

    // allocate memory to contain the whole file:
    buffer3 = (unsigned char*) malloc(sizeof (unsigned char)*lSize3);
    if (buffer3 == NULL) {
      fputs("Memory error", stderr);
      exit(2);
    }
    // copy the file into the buffer3:
    result3 = fread(buffer3, 1, lSize3, pFile3);
    if (result3 != lSize3) {
      fputs("Reading error", stderr);
      exit(3);
    }
  }
  
//  {
//    cout << "----------------------------------------------" << endl;
//    cout << "lSize  : " << lSize << "_|" << endl;
//    cout << "buffer : " << buffer << "_|" << endl;
//    cout << "lSize2 : " << lSize2 << "_|" << endl;
//    cout << "buffer2: " << buffer2 << "_|" << endl;
//    cout << "lSize3 : " << lSize3 << "_|" << endl;
//    cout << "buffer3: " << buffer3 << "_|" << endl;
//    cout << "----------------------------------------------" 
//  << endl << endl << endl << endl;
//  }
  
  cout << "----------------------------------------------" << endl;
//  printbuffer(lSize, buffer);
//  printbuffer(lSize2, buffer2);
//  printbuffer(lSize3, buffer3);
//  printJPG(lSize, buffer);
  printPNG(lSize2, buffer2);
//  printBMP(lSize3, buffer3);
  cout << "----------------------------------------------" << endl;
  
  // terminate
  fclose(pFile);
  free(buffer);
  
  fclose(pFile2);
  free(buffer2);
  
  fclose(pFile3);
  free(buffer3);
  return 0;
}

#define SCREEN_FACTOR 65

void printBIT(long i, unsigned char * buffer){
  printf("buffer[%6d]:   %02X\n", i, buffer[i]);
}

long printBITS(long i, long j, unsigned char * buffer){
  unsigned long sum = 0;
  
  printf("buffer[%6d-%6d]:   ", i, j);
  
  if(j > i) {
    for(long k = i; k < j; k++){
      printf("%02X", buffer[k]);
      sum *= 256;
      sum += (unsigned long)buffer[k];
    }
      
  }
  else {
    for(long k = i-1; k > j-1; k--){
      printf("%02X", buffer[k]);
      sum *= 256;
      sum += (unsigned long)buffer[k];
    }
  }

  printf("\n                  sum:            = %lu\n", sum);
  return sum;
}

void printbuffer(long lSize, unsigned char * buffer){
  for (long i = 0; i < lSize; i+=SCREEN_FACTOR) {
    for (long j = i; j < i+SCREEN_FACTOR && j < lSize; j ++)
      printBIT(j, buffer);
    char g;
    cin >> g;
  }
  return;
}

void printBMP(long lSize, unsigned char * buffer){
  cout << "Going to print BMP" << endl << endl;

  cout << "BMP Header" << endl;
  long signature = printBITS(0, 2, buffer);
  long fileSize = printBITS(6, 2, buffer);
  printBITS(6, 8, buffer); // Reserved1
  printBITS(8, 10, buffer); // Reserved2
  long offsetPixelArray = printBITS(14, 10, buffer);
  cout << endl;
  
  
  cout << "DIB Header" << endl;
  long dibHeaderSize = printBITS(18, 14, buffer);
  long imageWidth = printBITS(22, 18, buffer);
  long imageHeight = printBITS(26, 22, buffer);
  printBITS(28, 26, buffer); /// Planes
  long bitsPerPixel = printBITS(30, 28, buffer);
  long compression = printBITS(34, 30, buffer);
  long imageSize = printBITS(38, 34, buffer);
  cout << endl;
  
  
  cout << "Headers finished------------------------------" << endl;
  long endPixelArray = offsetPixelArray + imageSize;
  cout << "offsetPixelArray: " << offsetPixelArray << endl;
  cout << "imageSize: " << imageSize << endl;
  cout << "endPixelArray: " << endPixelArray << endl << endl;
  
  
  cout << "Pixel array parameters------------------------" << endl;
  long bytesPerPixel = bitsPerPixel / 8;
  long lineSize = imageSize/imageHeight;
  long paddingPerLine = lineSize % bitsPerPixel;
  
  cout << "bitsPerPixel: " << bitsPerPixel << endl;
  cout << "bytesPerPixel: " << bytesPerPixel << endl;
  cout << "imageWidth: " << imageWidth << endl;
  cout << "imageHeight: " << imageHeight << endl;
  cout << "lineSize: " << lineSize << endl;
  cout << "paddingPerLine: " << paddingPerLine << endl;
  cout << endl;
  
  assert(lineSize-paddingPerLine == imageWidth*bytesPerPixel);
  
  cout << "Start the pixel array printing----------------" << endl;
  for(long i = offsetPixelArray; i < endPixelArray; i+=lineSize){
    
    for(long j = i; j < i+lineSize-paddingPerLine; j+=bytesPerPixel){
      printBITS(j, j+bytesPerPixel, buffer);
      cout << "  ";
    }
    char g;
    cin >> g;
    
    cout << endl;
//    printBITS(i, i+imageWidth, buffer);
    
    
  }
  
  return;
}

void printJPG(long lSize, unsigned char * buffer){
  cout << "Going to print JPG" << endl << endl;
  
}

void printPNG(long lSize, unsigned char * buffer){
  cout << "Going to print PNG" << endl << endl;
  
  cout << "PNG Header" << endl;
  long transmissionSystem = printBITS(0, 1, buffer);
  cout << "transmissionSystem: " << transmissionSystem << endl << endl;
  long signature  = printBITS(1, 4, buffer);
  cout << "signature: " << signature << endl << endl;
  long crlf = printBITS(4, 6, buffer);
  cout << "crlf: " << crlf << endl << endl;
  long endoffile = printBITS(6, 7, buffer);
  cout << "endoffile: " << endoffile << endl << endl;
  long lf = printBITS(7, 8, buffer);
  cout << "lf: " << lf << endl << endl;
  
//  for(int i = 8; i < 100; i+=13){
//    long nextThirteen = printBITS(i, i+13, buffer);
//    cout << "nextThirteen: " << nextThirteen << endl << endl;
//  }

  long chunkIndex = PNG_HEADER_SIZE;
  
  char userin = 'p';
  
  while(chunkIndex < lSize && userin != 'q'){
    cout << "----------------------------------------------" << endl;
    cout << "chunk start" << endl;
    
    cout << "test: " << 255 / 16 << endl;
    
    long length = printBITS(chunkIndex, chunkIndex+4, buffer);
    char lengthBuffer[4];
    
    cout << "length: " << length << endl << endl;
    chunkIndex+=4;

    long chunktype = printBITS(chunkIndex, chunkIndex+4, buffer);
    cout << "chunktype: " << chunktype << endl << endl;
    chunkIndex+=4;

    long chunkdata = printBITS(chunkIndex, chunkIndex+length, buffer);
    cout << "chunkdata: " << chunkdata << endl << endl;
    chunkIndex+=length;

    long crc = printBITS(chunkIndex, chunkIndex+4, buffer);
    cout << "crc: " << crc << endl << endl;
    chunkIndex+=4;
    
    cout << "chunk ended" << endl << "type q to quit anything else to continue: ";
    cin >> userin;
  }
  
}