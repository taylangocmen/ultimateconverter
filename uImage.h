
/* 
 * File:   uImage.h
 * Author: gocmenta
 *
 * Created on March 13, 2017, 10:24 PM
 */

#ifndef UIMAGE_H
#define UIMAGE_H

#include <stdio.h>

#include "uUtils.h"

class uImage {
public:
    uImage();
    uImage(const char* fileName, format fmt);
    uImage(const uImage& orig);
    virtual ~uImage();
    void convert(const char* fileName, format fmt);
    void print_array();
    
private:
    unsigned short*** pxArr;
    unsigned pxHeight;
    unsigned pxWidth;
    unsigned pxFormat;

    FILE* file;
    char* fName;
    unsigned char* fBuffer;
    size_t fSize;
    format fFormat;

    void abort_(const char* s, ...);
    void delete_all();
    void check_status();
    
    void decipher_bmp();
    void decipher_jpg();
    void decipher_png();

    void write_bmp(const char* fileName);
    void write_jpg(const char* fileName);
    void write_png(const char* fileName);

    unsigned get_bytes(unsigned i, unsigned j);

};

#endif /* UIMAGE_H */

