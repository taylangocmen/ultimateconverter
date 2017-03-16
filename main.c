
#include <stdio.h>
#include <stdlib.h>

#include "uc_bmp.h"
#include "uc_image.h"
#include "uc_png.h"
#include "uc_utils.h"

int main(int argc, char** argv) {

    printf("This is the tester with c starting\n");

    convert_image("bmp_testrun1.bmp", bmp, "bmp_testrun1_to_png.png", png);
    convert_image("png_testrun1.png", png, "png_testrun1_to_png.png", png);
    
    return (EXIT_SUCCESS);
}

