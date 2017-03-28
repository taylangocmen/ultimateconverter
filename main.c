
#include <stdio.h>
#include <stdlib.h>

#include "uc_bmp.h"
#include "uc_image.h"
#include "uc_png.h"
#include "uc_utils.h"
#include "lodepng.h"

void bmp_to_png_test(void);
void png_to_bmp_test(void);

int main(int argc, char** argv) {
    
    

//    convert_image("bmp_testrun1.bmp", bmp, "bmp_testrun1_to_png.png", png);
//    convert_image("png_testrun1.png", png, "png_testrun1_to_bmp.bmp", bmp);
//    convert_image("png_testrun1.png", png, "png_testrun1_to_png.png", png);
//    convert_image("bmp_testrun1_to_png.png", png, "bmp_testrun1_to_png_to_bmp.bmp", bmp);
    bmp_to_png_test();
    png_to_bmp_test();
   

//    UC_IMAGE* testBmp;
//    testBmp = open_uc_image("jpg_testrun4.jpg", jpg);
////    write_uc_image(testBmp, "jpg_testrun1_to_png.png", png);
//    close_uc_image(testBmp);
//    free(testBmp);
    
    return (EXIT_SUCCESS);
}

void bmp_to_png_test(void){
    unsigned i;
    
    FILE* fromFile;
    fromFile = fopen("bmp_testrun1.bmp", "rb");
    
    fseek(fromFile, 0, SEEK_END);
    size_t fromSize;
    fromSize = ftell(fromFile);
    rewind(fromFile);

    unsigned char* fromBuffer;
    fromBuffer = (unsigned char*) malloc(fromSize * sizeof (unsigned char));
    fread(fromBuffer, 1, fromSize, fromFile);
    
    size_t toSize;
    unsigned char* toBuffer;
    toBuffer = (unsigned char*) malloc(2000000);
    
    toSize = (size_t)convert_image((volatile int *)fromBuffer, fromSize, bmp, (volatile int *)toBuffer, png);
    
    FILE* toFile;
    toFile = fopen("bmp_testrun1_to_png.png", "wb");
    
    for(i = 0; i < toSize; i++)
        fputc(toBuffer[i], toFile);
    
    fclose(toFile);
    free(toBuffer);

    fclose(fromFile);
    free(fromBuffer);
}

void png_to_bmp_test(void){
    
    unsigned i;
    
    FILE* fromFile;
    fromFile = fopen("png_testrun1.png", "rb");
    
    fseek(fromFile, 0, SEEK_END);
    size_t fromSize;
    fromSize = ftell(fromFile);
    rewind(fromFile);

    unsigned char* fromBuffer;
    fromBuffer = (unsigned char*) malloc(fromSize * sizeof (unsigned char));
    fread(fromBuffer, 1, fromSize, fromFile);
    
    size_t toSize;
    unsigned char* toBuffer;
    toBuffer = (unsigned char*) malloc(2000000);
    
    toSize = (size_t)convert_image((volatile int *)fromBuffer, fromSize, png, (volatile int *)toBuffer, bmp);
    
    FILE* toFile;
    toFile = fopen("png_testrun1_to_bmp.bmp", "wb");
    
    for(i = 0; i < toSize; i++)
        fputc(toBuffer[i], toFile);
    
    fclose(toFile);
    free(toBuffer);

    fclose(fromFile);
    free(fromBuffer);
}