
/* 
 * File:   uImage.cpp
 * Author: gocmenta
 * 
 * Created on March 13, 2017, 10:24 PM
 */

#include "uImage.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <png.h>

#include "uUtils.h"
#include "uPNG.h"
#include "uBMP.h"

using namespace std;

uImage::uImage() {
}

uImage::uImage(const char* fileName, format fmt) {

    file = fopen(fileName, "rb");
    if (file == NULL)
        abort_("Cannot open %s", fileName);

    fseek(file, 0, SEEK_END);
    fSize = ftell(file);
    rewind(file);

    fBuffer = new unsigned char[fSize];
    if (fBuffer == NULL)
        abort_("Memory error allocating for %s", fileName);

    size_t result = fread(fBuffer, 1, fSize, file);
    if (result != fSize)
        abort_("Cannot read %s", fileName);

    fName = (char *) fileName;

    fFormat = fmt;

    if (fmt == bmp) decipher_bmp();
    else if (fmt == jpg) decipher_jpg();
    else if (fmt == png) decipher_png();

}

uImage::uImage(const uImage& orig) {
}

uImage::~uImage() {

    delete_all();
}

void uImage::convert(const char* fileName, format fmt) {

    check_status();

    if (fmt == bmp) write_bmp(fileName);
    else if (fmt == jpg) write_jpg(fileName);
    else if (fmt == png) write_png(fileName);
    //TODO: do the writebacks
}

void uImage::print_array() {
    for (unsigned i = 0; i < pxHeight; i++) {
        for (unsigned j = 0; j < pxWidth; j++) {
            for (unsigned k = 0; k < 4; k++) {
                cout << "pxArr[" << i << "][" << j << "][" << k << "]: " << pxArr[i][j][k] << endl;
            }
        }
    }
}

void uImage::abort_(const char* s, ...) {
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);

    delete_all();
    abort();
}

void uImage::delete_all() {

    if (fBuffer != NULL) {
        delete[] fBuffer;
        fBuffer = NULL;
    }

    fclose(file);
    file = NULL;

    if (pxArr != NULL) {
        for (unsigned i = 0; i < pxHeight; i++) {
            for (unsigned j = 0; j < pxWidth; j++) {
                delete[] pxArr[i][j];
            }
            delete[] pxArr[i];
        }
        delete[] pxArr;
        pxArr = NULL;
    }

}

void uImage::check_status() {

    //TODO: fill this
}

void uImage::decipher_bmp() {
    unsigned signature = get_bytes(0, 2);
    if (signature != BMP_BM)
        abort_("File %s was to be a BMP but was not", fName);

    unsigned offsetPxArr = get_bytes(14, 10);
    unsigned dibHSize = get_bytes(18, 14);
    pxWidth = get_bytes(22, 18);
    pxHeight = get_bytes(26, 22);

    if (dibHSize != DIB_CORE && dibHSize != DIB_INFO && dibHSize != DIB_V4 && dibHSize != DIB_V5)
        abort_("File: %s has an unsupported DIBHEADER size: %u ", fName, dibHSize);

    assert(dibHSize >= DIB_CORE);
    if (dibHSize == DIB_CORE) {
        abort_("We don't know what to do with DIB_CORE yet file: %s", fName);
        return;
    }

    unsigned bitsPPx = get_bytes(30, 28);
    unsigned imageSize = get_bytes(38, 34);
    unsigned colorsInTable = get_bytes(50, 46);
    unsigned short** colorTable;

    pxFormat = 3;
    
    if (bitsPPx == BMP_1BPP || bitsPPx == BMP_2BPP || bitsPPx == BMP_4BPP || bitsPPx == BMP_8BPP) {
        assert(colorsInTable == to_power(2, bitsPPx));
        colorTable = new unsigned short*[colorsInTable];
        for (unsigned i = 0; i < colorsInTable; i++) colorTable[i] = new unsigned short[4];
        unsigned index = BMP_HEADER_SIZE + dibHSize;
        for (unsigned j = 0; j < colorsInTable; index += 4, j++) {
            // Not sure about this colors
            colorTable[j][2] = get_bytes(index + 1, index + 0); // blue
            colorTable[j][1] = get_bytes(index + 2, index + 1); // green
            colorTable[j][0] = get_bytes(index + 3, index + 2); // red
            colorTable[j][3] = get_bytes(index + 4, index + 3); // alpha
        }
    } else if (bitsPPx == BMP_16BPP || bitsPPx == BMP_24BPP || bitsPPx == BMP_32BPP) {
        abort_("We don't know what to do with this BPP yet file: %s", fName);
    } else abort_("File: %s has an unsupported BPP", fName);


    unsigned endPxArr = offsetPxArr + imageSize;
    unsigned bytesPPx = bitsPPx / 8;
    unsigned lineSize = imageSize / pxHeight;
    unsigned paddingPLine = lineSize % bytesPPx;
    assert(lineSize - paddingPLine == pxWidth * bytesPPx);

    pxArr = new unsigned short**[pxHeight];
    for (unsigned i = 0; i < pxHeight; i++) {
        pxArr[i] = new unsigned short*[pxWidth];
        for (unsigned j = 0; j < pxWidth; j++) {
            pxArr[i][j] = new unsigned short[4];
        }
    }

    assert(dibHSize >= DIB_INFO);
    if (dibHSize == DIB_INFO) {
        unsigned rowIndex = pxHeight - 1;
        for (unsigned i = offsetPxArr; i < endPxArr; i += lineSize, rowIndex--) {
            unsigned colIndex = 0;
            for (unsigned j = i; j < i + lineSize - paddingPLine; j += bytesPPx, colIndex++) {
                unsigned pixel = get_bytes(j, j + bytesPPx);
                pxArr[rowIndex][colIndex][0] = colorTable[pixel][0];
                pxArr[rowIndex][colIndex][1] = colorTable[pixel][1];
                pxArr[rowIndex][colIndex][2] = colorTable[pixel][2];
                pxArr[rowIndex][colIndex][3] = colorTable[pixel][3];
            }
            assert(colIndex == pxWidth);
        }
        assert(rowIndex == 4294967295);
        return;
    }

    assert(bitsPPx == BMP_16BPP || bitsPPx == BMP_24BPP || bitsPPx == BMP_32BPP);

    assert(dibHSize >= DIB_V4);
    abort_("We don't know what to do with DIB_V4 yet file: %s", fName);
    if (dibHSize == DIB_V4) {
        return;
    }

    assert(dibHSize == DIB_V5);
    abort_("We don't know what to do with DIB_V5 yet file: %s", fName);
    return;
}

void uImage::decipher_jpg() {

}

void uImage::decipher_png() {

    fseek (file, 8, SEEK_SET);
    if (png_sig_cmp(fBuffer, 0, 8))
        abort_("[read_png_file] File %s is not recognized as a PNG file", fName);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        abort_("[read_png_file] png_create_read_struct failed");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        abort_("[read_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[read_png_file] Error during init_io");

    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    pxWidth = png_get_image_width(png_ptr, info_ptr);
    pxHeight = png_get_image_height(png_ptr, info_ptr);
    //  color_type = png_get_color_type(png_ptr, info_ptr);
    //  bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);


    pxArr = new unsigned short**[pxHeight];
    for (unsigned i = 0; i < pxHeight; i++) {
        pxArr[i] = new unsigned short*[pxWidth];
        for (unsigned j = 0; j < pxWidth; j++) {
            pxArr[i][j] = new unsigned short[4];
        }
    }

    /* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[read_png_file] Error during read_image");

    png_bytep* row_pointers = (png_bytep*) malloc(sizeof (png_bytep) * pxHeight);
    for (unsigned y = 0; y < pxHeight; y++)
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));

    png_read_image(png_ptr, row_pointers);

    pxFormat = (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) ? 3 : 4;

    for (unsigned y = 0; y < pxHeight; y++) {
        png_byte* row = row_pointers[y];
        for (unsigned x = 0; x < pxWidth; x++) {
            png_byte* ptr = &(row[x * pxFormat]);
            pxArr[y][x][0] = ptr[0];
            pxArr[y][x][1] = ptr[1];
            pxArr[y][x][2] = ptr[2];
            if(pxFormat == 4)
                pxArr[y][x][3] = ptr[3];
        }
    }
}

void uImage::write_bmp(const char* fileName) {

}

void uImage::write_jpg(const char* fileName) {

}

void uImage::write_png(const char* fileName) {
    png_bytep* row_pointers = (png_bytep*) malloc(sizeof (png_bytep) * pxHeight);
    for (unsigned y = 0; y < pxHeight; y++)
        row_pointers[y] = (png_byte*) malloc(pxFormat * pxWidth);

    for (unsigned y = 0; y < pxHeight; y++) {
        png_byte* row = row_pointers[y];
        for (unsigned x = 0; x < pxWidth; x++) {
            png_byte* ptr = &(row[x * pxFormat]);
            ptr[0] = pxArr[y][x][0];
            ptr[1] = pxArr[y][x][1];
            ptr[2] = pxArr[y][x][2];
            if(pxFormat == 4)
                ptr[3] = pxArr[y][x][3];
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

    png_set_IHDR(png_ptr, info_ptr, (int)pxWidth, (int)pxHeight,
        bit_depth, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing bytes");
    png_write_image(png_ptr, row_pointers);

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during end of write");
    png_write_end(png_ptr, NULL);

    for (unsigned y = 0; y < pxHeight; y++)
        free(row_pointers[y]);
    free(row_pointers);

    fclose(file);
}

unsigned uImage::get_bytes(unsigned i, unsigned j) {
    // TODO: this is not finsihed at all
    unsigned sum = 0;

    if (j > i) {

        for (unsigned k = i; k < j; k++) {
            sum *= 256;
            sum += (unsigned) fBuffer[k];
        }
    } else if (i > j) {

        for (unsigned k = i - 1; k > j - 1; k--) {
            sum *= 256;
            sum += (unsigned) fBuffer[k];
        }
    } else
        abort_("Tried to get 0 bytes i: %u j: % u file: %s", i, j, fName);

    return sum;
}
