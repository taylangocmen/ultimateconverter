
/* 
 * File:   main.cpp
 * Author: gocmenta
 *
 * Created on March 2, 2017, 12:25 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <cstring>
#include <string>
#include "zlib.h"

#include "utils.h"
#include "bmp.h"
#include "png.h"

using namespace std;

/*
 * 
 */

void do_bmp(void) {
  const char* bmpName = "originalBMP.bmp";
  uImage* bmpImage = open_file(bmpName);
  print_bmp(bmpImage);
  close_file(bmpImage);
  free(bmpImage);
}

void do_png(char* file_name) {
  const char* pngName = file_name; //"originalPNG.png";
  uImage* pngImage = open_file(pngName);
  print_png(pngImage);
  close_file(pngImage);
  free(pngImage);
}

void do_test(void) {
  char a[500] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  char b[500];
  char c[500];

  uLong ucompSize = strlen(a) + 1; // "Hello, world!" + NULL delimiter.
  uLong compSize = compressBound(ucompSize);

  // Deflate
  compress((Bytef *) b, &compSize, (Bytef *) a, ucompSize);

  // Inflate
  uncompress((Bytef *) c, &ucompSize, (Bytef *) b, compSize);

  cout << "ucompSize: " << ucompSize << endl;
  cout << "compSize: " << compSize << endl;


  cout << "a: " << a << endl;
  cout << "b: " << b << endl;
  cout << "c: " << c << endl;
}

int main(int argc, char** argv) {

  //    do_bmp();
  //    do_png();
  //  do_test();


  string input = "";

  while (input != "q") {
    cout << "INPUT: ";
    getline(cin, input);

    if(input == "png")
      do_png((char *) "originalPNG.png");
    
    else if(input == "bmp")
      do_bmp();
    
    else if(input == "half")
      do_png((char *) "halfandhalf.png");
    
    else if(input == "white")
      do_png((char *) "solidwhite.png");
     
    else if(input == "black")
      do_png((char *) "solidblack.png");
        
    else if(input == "string")
      do_test();
  }

  // nikita see here sample
  //  char a[500] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  //  char* b = compress_string(a, 500);
  //  char* c = decompress_string(b, 500);
  //  
  //  cout << "original     string a: " << a << endl ;
  //  cout << "compressed   string b: " << b << endl ;
  //  cout << "decompressed string c: " << c << endl ;
  //  
  //  delete b;
  //  delete c;

  return 0;
}

