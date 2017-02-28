#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void printBIT(long i, unsigned char * buffer);
long printBITS(long i, long j, unsigned char * buffer);
void printbuffer(long lSize, unsigned char * buffer);
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
//    cout << "----------------------------------------------" << endl << endl << endl << endl;
//  }
  
  cout << "----------------------------------------------" << endl;
//  printbuffer(lSize, buffer);
//  printbuffer(lSize2, buffer2);
//  printbuffer(lSize3, buffer3);
  printBMP(lSize3, buffer3);
  cout << "----------------------------------------------" << endl << endl << endl << endl;
  
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
  printf("buffer[%6d]:   %02X_|\n", i, buffer[i]);
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
  return j;
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
  cout << "Going to print BMP" << endl;
  cout << endl;

  long i = 0;
  
  cout << "BMP Header" << endl;
  printBITS(0, 2, buffer);
  printBITS(6, 2, buffer);
  printBITS(6, 8, buffer);
  printBITS(8, 10, buffer);
  printBITS(14, 10, buffer);
  cout << endl;
  
  cout << "DIB Header" << endl;
  printBITS(18, 14, buffer);
  printBITS(22, 18, buffer);
  printBITS(26, 22, buffer);
  printBITS(28, 26, buffer);
  printBITS(30, 28, buffer);
  printBITS(34, 30, buffer);
  printBITS(38, 34, buffer);
  
  cout << endl;
  
  return;
}