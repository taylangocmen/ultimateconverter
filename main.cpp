
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
#include "uUtils.h"
#include "uc_png.h"
#include "pngsample.h"
#include "uImage.h"

using namespace std;

/*
 * 
 */

void do_png(char* file_name) {
//      const char* pngName = file_name; //"originalPNG.png";
//      uFile* pngImage = open_file(pngName);
//      print_png(pngImage);
//      close_file(pngImage);
//      free(pngImage);
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

    uImage* testBmp;
    testBmp = new uImage("bmp_testrun1.bmp", bmp);
    testBmp->convert("bmp_testrun1_converted_png.png", png);
    delete testBmp;
    
    return 0;
}

