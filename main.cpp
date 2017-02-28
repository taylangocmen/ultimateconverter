#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <bitset>

using namespace std;

int main() {
  cout << "----------------------------------------------" << endl;
  cout << "Start of program" << endl;
  cout << "----------------------------------------------" << endl << endl << endl << endl;
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
  
  
  /*********************               JPG                *********************/
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
  
  
  
  
  
  /*********************               PNG                *********************/
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
  
  
  
  
  
  /*********************               BMP                *********************/
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
  
  
  
  
  
  
  cout << "----------------------------------------------" << endl;
  cout << "lSize  : " << lSize << "_|" << endl;
  cout << "buffer : " << buffer << "_|" << endl;
//  for (int i; i < lSize; i++) {
//    cout << "i: " << i << "  ---  " << "buffer[i]: ";
//    printf("0x%02x", buffer[i]);
//    cout << "_|" << endl;
//  }
  cout << "----------------------------------------------" << endl << endl << endl << endl;
  
  
  cout << "----------------------------------------------" << endl;
  cout << "lSize2 : " << lSize2 << "_|" << endl;
  cout << "buffer2: " << buffer2 << "_|" << endl;
//  for (int i; i < lSize2; i++) {
//    cout << "i: " << i << "  ---  " << "buffer[i]: ";
//    printf("0x%02x", buffer2[i]);
//    cout << "_|" << endl;
//  }
  cout << "----------------------------------------------" << endl << endl << endl << endl;
  
  
  cout << "----------------------------------------------" << endl;
  cout << "lSize3 : " << lSize3 << "_|" << endl;
  cout << "buffer3: " << buffer3 << "_|" << endl;
//  for (int i; i < lSize3; i++) {
//    cout << "i: " << i << "  ---  " << "buffer3[i]: ";
//    printf("0x%02x", buffer3[i]);
//    cout << "_|" << endl;
//  }
  cout << "----------------------------------------------" << endl << endl << endl << endl;
  
  
  
  // terminate
  fclose(pFile);
  free(buffer);
  
  fclose(pFile2);
  free(buffer2);
  
  fclose(pFile3);
  free(buffer3);
  cout << "----------------------------------------------" << endl;
  cout << "End of program" << endl;
  cout << "----------------------------------------------" << endl;
  return 0;
}