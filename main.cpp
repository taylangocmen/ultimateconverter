#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <bitset>

using namespace std;

int main() {
  cout << "----------------------------------------------" << endl;
  cout << "Start of program" << endl;
  cout << "----------------------------------------------" << endl;
  //  printf("Start of program");

  
  
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen("originalJPG.jpg", "rb");
  if (pFile == NULL) {
    fputs("File error", stderr);
    exit(1);
  }

  cout << "File OK" << endl;
  cout << "sizeof(pFile): " << sizeof (pFile) << "_|" << endl;
  cout << "pFile: " << pFile << "_|" << endl;
  cout << "----------------------------------------------" << endl;

  
  
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
  cout << "lSize: " << lSize << "_|" << endl;
  cout << "sizeof(buffer): " << sizeof (buffer) << "_|" << endl;
  cout << "buffer: " << buffer << "_|" << endl;
  cout << "----------------------------------------------" << endl;

  
  
  // copy the file into the buffer:
  result = fread(buffer, 1, lSize, pFile);
  if (result != lSize) {
    fputs("Reading error", stderr);
    exit(3);
  }

  cout << "Reading OK" << endl;
  cout << "result: " << result << "_|" << endl;
  cout << "sizeof(buffer): " << sizeof (buffer) << "_|" << endl;
  cout << "buffer: " << buffer << "_|" << endl;
  cout << "----------------------------------------------" << endl;

  

  /* the whole file is now loaded in the memory buffer. */
  cout << "lSize: " << lSize << "_|" << endl;
  for (int i; i < lSize; i++) {
    cout << "i: " << i << "  ---  " << "buffer[i]: " << bitset<8>(buffer[i]) << "_|" << endl;
  }
  cout << "----------------------------------------------" << endl;

  
  
  // terminate
  fclose(pFile);
  free(buffer);

  
  
  cout << "End of program" << endl;
  cout << "----------------------------------------------" << endl;
  //  printf("End of program");
  return 0;
}