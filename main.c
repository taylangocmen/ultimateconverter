
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

    return (EXIT_SUCCESS);
}

