
#ifndef UC_IMAGE_H
#define UC_IMAGE_H

#include "uc_utils.h"

//typedef struct ultimate_converter_image {

typedef struct UC_IMAGE {
    unsigned short*** pxArr;
    unsigned pxHeight;
    unsigned pxWidth;
    unsigned pxFormat;

    FILE* file;
    char* fName;
    unsigned char* fBuffer;
    size_t fSize;
    unsigned fFormat;
} UC_IMAGE;

//typedef struct UC_IMAGE UC_IMAGE;


UC_IMAGE* open_uc_image(const char* fileName, unsigned fmt);
void write_uc_image(UC_IMAGE* image, const char* fileName, unsigned fmt);
void close_uc_image(UC_IMAGE* image);

void convert_image(const char* fromName, unsigned fromFmt, const char* toName, unsigned toFmt);

#endif /* UC_IMAGE_H */

