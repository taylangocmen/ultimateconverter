
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
#include <png.h>
#include "zlib.h"

#include "utils.h"
#include "uc_bmp.h"
#include "uc_png.h"
#include "pngsample.h"
#include "testrun.h"

using namespace std;

/*
 * 
 */

void do_bmp(char* file_name) {
  const char* bmpName = file_name;
  uImage* bmpImage = open_file(bmpName);
  print_bmp(bmpImage);
  close_file(bmpImage);
  free(bmpImage);
}

void do_png(char* file_name) {
//  const char* pngName = file_name; //"originalPNG.png";
//  uImage* pngImage = open_file(pngName);
//  print_png(pngImage);
//  close_file(pngImage);
//  free(pngImage);
}

void do_test(void) {
  char a[500] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
  "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim "
  "ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip "
  "ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate"
  " velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat "
  "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id "
  "est laborum.";
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

int main(void) {
  
  cout << "sizeof(char)" << sizeof(char) << endl;
  cout << "sizeof(unsigned)" << sizeof(unsigned) << endl;
  cout << "sizeof(int)" << sizeof(int) << endl;
  cout << "sizeof(unsigned int)" << sizeof(unsigned int) << endl;
  cout << "sizeof(long)" << sizeof(long) << endl;
  cout << "sizeof(unsigned long)" << sizeof(unsigned long) << endl;

//  string input = "";
//  
//  png_bytep * testrunone = testrun1();
//  png_bytep * pngsampleone = pngsample();
//  
//
//  
//  for (int y = 0; y < 768; y++) {
//    png_byte* row1 = testrunone[y];
//    png_byte* row2 = pngsampleone[y];
//    for (int x = 0; x < 1024; x++) {
//      png_byte* ptr1 = &(row1[x * 3]);
//      png_byte* ptr2 = &(row2[x * 3]);
////      printf("..testrunone [ %d - %d ] has RGB values: %d - %d - %d\n",
////        x, y, ptr1[0], ptr1[1], ptr1[2]);
////      printf("pngsampleone [ %d - %d ] has RGB values: %d - %d - %d\n",
////        x, y, ptr2[0], ptr2[1], ptr2[2]); if(x == 1023) printf("\n");
//      
//    }
//  }
  
  //  do_bmp("bmp_testrun1.bmp");
//  do_png((char *) "png_small.png");


//  while (input != "q") {
//    cout << "INPUT: ";
//    getline(cin, input);
//
//    if(input == "png")
//      do_png((char *) "originalPNG.png");
//    
//    else if(input == "bmp")
//      do_bmp((char *) "originalBMP.bmp");
//    
//    else if(input == "half")
//      do_png((char *) "png_halfandhalf.png");
//    
//    else if(input == "white")
//      do_png((char *) "png_solidwhite.png");
//     
//    else if(input == "black")
//      do_png((char *) "png_solidblack.png");
//        
//    else if(input == "string")
//      do_test();
//    
//    else if(input == "smallpng")
//      do_png((char *) "png_small.png");
//    
//    else if(input == "pngsample")
//      pngsample();
//  }

  return 0;
}

