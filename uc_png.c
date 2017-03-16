
#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#include "uc_bmp.h"
#include "uc_image.h"
#include "uc_png.h"
#include "uc_utils.h"

void read_png(UC_IMAGE* image) {

    unsigned i, j;

    fseek(image->file, 8, SEEK_SET);
    if (png_sig_cmp(image->fBuffer, 0, 8))
        abort_("[read_png_file] File %s is not recognized as a PNG file", image->fName);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        abort_("[read_png_file] png_create_read_struct failed");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        abort_("[read_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[read_png_file] Error during init_io");

    png_init_io(png_ptr, image->file);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    image->pxWidth = png_get_image_width(png_ptr, info_ptr);
    image->pxHeight = png_get_image_height(png_ptr, info_ptr);
    //  color_type = png_get_color_type(png_ptr, info_ptr);
    //  bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);


    image->pxArr = (unsigned short***) malloc(image->pxHeight * sizeof (unsigned short**));
    for (i = 0; i < image->pxHeight; i++) {
        image->pxArr[i] = (unsigned short**) malloc(image->pxWidth * sizeof (unsigned short*));
        for (j = 0; j < image->pxWidth; j++) {
            image->pxArr[i][j] = (unsigned short*) malloc(4 * sizeof (unsigned short));
        }
    }

    /* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[read_png_file] Error during read_image");

    png_bytep* row_pointers = (png_bytep*) malloc(sizeof (png_bytep) * image->pxHeight);
    for (j = 0; j < image->pxHeight; j++)
        row_pointers[j] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));

    png_read_image(png_ptr, row_pointers);

    image->pxFormat = (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) ? 3 : 4;

    for (j = 0; j < image->pxHeight; j++) {
        png_byte* row = row_pointers[j];
        for (i = 0; i < image->pxWidth; i++) {
            png_byte* ptr = &(row[i * image->pxFormat]);
            image->pxArr[j][i][0] = ptr[0];
            image->pxArr[j][i][1] = ptr[1];
            image->pxArr[j][i][2] = ptr[2];
            if (image->pxFormat == 4)
                image->pxArr[j][i][3] = ptr[3];
        }
    }
}

void write_png(UC_IMAGE* image, const char* fileName) {
    unsigned i, j;

    png_bytep* row_pointers = (png_bytep*) malloc(sizeof (png_bytep) * image->pxHeight);
    for (j = 0; j < image->pxHeight; j++)
        row_pointers[j] = (png_byte*) malloc(image->pxFormat * image->pxWidth);

    for (j = 0; j < image->pxHeight; j++) {
        png_byte* row = row_pointers[j];
        for (i = 0; i < image->pxWidth; i++) {
            png_byte* ptr = &(row[i * image->pxFormat]);
            ptr[0] = image->pxArr[j][i][0];
            ptr[1] = image->pxArr[j][i][1];
            ptr[2] = image->pxArr[j][i][2];
            if (image->pxFormat == 4)
                ptr[3] = image->pxArr[j][i][3];
        }
    }

    png_byte color_type = '\x02';
    png_byte bit_depth = '\x08';
    png_structp png_ptr;
    png_infop info_ptr;

    FILE *file = fopen(fileName, "wb");
    if (!file)
        abort_("[write_png_file] File %s could not be opened for writing", fileName);
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        abort_("[write_png_file] png_create_write_struct failed");
    info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr)
        abort_("[write_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during init_io");
    png_init_io(png_ptr, file);

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing header");

    png_set_IHDR(png_ptr, info_ptr, (int) (image->pxWidth), (int) (image->pxHeight),
        bit_depth, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing bytes");
    png_write_image(png_ptr, row_pointers);

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during end of write");
    png_write_end(png_ptr, NULL);

    for (j = 0; j < image->pxHeight; j++)
        free(row_pointers[j]);
    free(row_pointers);

    fclose(file);
}