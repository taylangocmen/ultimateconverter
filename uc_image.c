
#include <stdio.h>
#include <stdlib.h>

#include "uc_bmp.h"
#include "uc_png.h"
#include "uc_jpg.h"
#include "uc_image.h"
#include "uc_utils.h"

UC_IMAGE* open_uc_image(volatile int *memAddr, size_t fileSize, const char* fileName, unsigned fmt) {
    UC_IMAGE* image;
    image = (UC_IMAGE*) malloc(sizeof (UC_IMAGE));

//    image->file = fopen(fileName, "rb");
//    if (image->file == NULL)
//        abort_("Cannot open %s", fileName);

//    fseek(image->file, 0, SEEK_END);
//    image->fSize = ftell(image->file);
//    rewind(image->file);

	image->fSize = fileSize;

    image->fBuffer = (unsigned char*) malloc(image->fSize * sizeof (unsigned char));
    if (image->fBuffer == NULL)
        abort_("Memory error allocating for %s", fileName);

//    size_t result = fread(image->fBuffer, 1, image->fSize, image->file);
//    if (result != image->fSize)
//        abort_("Cannot read %s", fileName);

//    image->fName = (char *) fileName;
	
	int i = 0;
	while(i < (unsigned int)fileSize)
	{
		unsigned int val = *memAddr;
		memAddr++;

		image->fBuffer[i] = val & 0xFF;
		image->fBuffer[i+1] = (val >> 8) & 0xFF;
		image->fBuffer[i+2] = (val >> 16) & 0xFF;
		image->fBuffer[i+3] = (val >> 24) & 0xFF;

		i += 4;
	}
	
    image->fFormat = fmt;

    if (fmt == bmp) read_bmp(image);
    else if (fmt == png) read_png(image);
    else if (fmt == jpg) read_jpg(image);

    return image;
}

void write_uc_image(UC_IMAGE* image, const char* fileName, unsigned fmt){
    if (fmt == bmp) write_bmp(image, fileName);
    else if (fmt == png) write_png(image, fileName);
    else if (fmt == jpg) write_jpg(image, fileName);
}

void close_uc_image(UC_IMAGE* image) {
    unsigned i, j;

    if (image->fBuffer != NULL) {
        free(image->fBuffer);
        image->fBuffer = NULL;
    }

//    fclose(image->file);
//    image->file = NULL;

    if (image->pxArr != NULL) {
        for (i = 0; i < image->pxHeight; i++) {
            for (j = 0; j < image->pxWidth; j++) {
                free(image->pxArr[i][j]);
            }
            free(image->pxArr[i]);
        }
        free(image->pxArr);
        image->pxArr = NULL;
    }
}

void convert_image(volatile int *memAddr, size_t fileSize, const char* fromName, unsigned fromFmt, const char* toName, unsigned toFmt){
    UC_IMAGE* testBmp;
    testBmp = open_uc_image(memAddr, fileSize, fromName, fromFmt);
    write_uc_image(testBmp, toName, toFmt);
    close_uc_image(testBmp);
    free(testBmp);
}
