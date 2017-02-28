#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main() {
  cout << "Start of program" << endl;
//  printf("Start of program");
  
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen("hello", "rb");
  if (pFile == NULL) {
    fputs("File error", stderr);
    exit(1);
  }
  
  cout << "File OK" << endl;

  // obtain file size:
  fseek(pFile, 0, SEEK_END);
  lSize = ftell(pFile);
  rewind(pFile);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc(sizeof (char)*lSize);
  if (buffer == NULL) {
    fputs("Memory error", stderr);
    exit(2);
  }

  cout << "Memory OK" << endl;
  
  // copy the file into the buffer:
  result = fread(buffer, 1, lSize, pFile);
  if (result != lSize) {
    fputs("Reading error", stderr);
    exit(3);
  }
  
  cout << "Reading OK" << endl;

  /* the whole file is now loaded in the memory buffer. */
  cout << "lSize: " << lSize << endl;
  cout << "sizeof(buffer): " << sizeof(buffer) << endl;
  cout << "buffer: " << buffer << "---|" << endl;
  
  
  
//  cout << "Bytebybyte" << endl;
  

  // terminate
  fclose(pFile);
  free(buffer);
  
  cout << "End of program" << endl;
//  printf("End of program");
  return 0;
}