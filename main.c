
#include <stdio.h>
#include <stdlib.h>

#include "uc_bmp.h"
#include "uc_image.h"
#include "uc_png.h"
#include "uc_utils.h"

int main(int argc, char** argv) {

//    convert_image("bmp_testrun1.bmp", bmp, "bmp_testrun1_to_png.png", png);
//    convert_image("png_testrun1.png", png, "png_testrun1_to_png.png", png);
    convert_image("bmp_testrun1_to_png.png", png, "bmp_testrun1_to_png_to_bmp.bmp", bmp);

//    UC_IMAGE* testBmp1;
//    testBmp1 = open_uc_image("bmp_testrun1.bmp", bmp);
//    UC_IMAGE* testBmp2;
//    testBmp2 = open_uc_image("bmp_testrun1_to_png_to_bmp.bmp", bmp);
//    
//    printf("testBmp1->pxHeight=%u\n", testBmp1->pxHeight);
//    printf("testBmp2->pxHeight=%u\n\n", testBmp2->pxHeight);
//    
//    printf("testBmp1->pxWidth=%u\n", testBmp1->pxWidth);
//    printf("testBmp2->pxWidth=%u\n\n", testBmp2->pxWidth);
//    
//    
//    close_uc_image(testBmp1);
//    free(testBmp1);
//    close_uc_image(testBmp2);
//    free(testBmp2);

    return (EXIT_SUCCESS);
}

