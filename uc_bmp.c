
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "uc_bmp.h"
#include "uc_image.h"
#include "uc_png.h"
#include "uc_utils.h"

void read_bmp(UC_IMAGE* image) {
    unsigned i, j;

    unsigned signature = get_bytes(0, 2, image->fBuffer);
    if (signature != BMP_BM)
        abort_("File %s was to be a BMP but was not", image->fName);

    unsigned offsetPxArr = get_bytes(14, 10, image->fBuffer);
    unsigned dibHSize = get_bytes(18, 14, image->fBuffer);
    image->pxWidth = get_bytes(22, 18, image->fBuffer);
    image->pxHeight = get_bytes(26, 22, image->fBuffer);

    if (dibHSize != DIB_CORE && dibHSize != DIB_INFO && dibHSize != DIB_V4 && dibHSize != DIB_V5)
        abort_("File: %s has an unsupported DIBHEADER size: %u ", image->fName, dibHSize);

    assert(dibHSize >= DIB_CORE);
    if (dibHSize == DIB_CORE) {
        abort_("We don't know what to do with DIB_CORE yet file: %s", image->fName);
        return;
    }

    unsigned bitsPPx = get_bytes(30, 28, image->fBuffer);
    unsigned imageSize = get_bytes(38, 34, image->fBuffer);
    unsigned colorsInTable = get_bytes(50, 46, image->fBuffer);
    unsigned short** colorTable;

    image->pxFormat = 3;

    if (bitsPPx == BMP_1BPP || bitsPPx == BMP_2BPP || bitsPPx == BMP_4BPP || bitsPPx == BMP_8BPP) {
        assert(colorsInTable == to_power(2, bitsPPx));
        colorTable = (unsigned short**) malloc(colorsInTable * sizeof (unsigned short*));

        for (i = 0; i < colorsInTable; i++)
            colorTable[i] = (unsigned short *) malloc(4 * sizeof (unsigned short));

        unsigned index = BMP_HEADER_SIZE + dibHSize;
        for (j = 0; j < colorsInTable; index += 4, j++) {
            // Not sure about this colors
            colorTable[j][2] = get_bytes(index + 1, index + 0, image->fBuffer); // blue
            colorTable[j][1] = get_bytes(index + 2, index + 1, image->fBuffer); // green
            colorTable[j][0] = get_bytes(index + 3, index + 2, image->fBuffer); // red
            colorTable[j][3] = get_bytes(index + 4, index + 3, image->fBuffer); // alpha
        }
    } else if (bitsPPx == BMP_16BPP || bitsPPx == BMP_24BPP || bitsPPx == BMP_32BPP) {
        abort_("We don't know what to do with this BPP yet file: %s", image->fName);
    } else abort_("File: %s has an unsupported BPP", image->fName);


    unsigned endPxArr = offsetPxArr + imageSize;
    unsigned bytesPPx = bitsPPx / 8;
    unsigned lineSize = imageSize / image->pxHeight;
    unsigned paddingPLine = lineSize % bytesPPx;
    assert(lineSize - paddingPLine == image->pxWidth * bytesPPx);

    image->pxArr = (unsigned short***) malloc(image->pxHeight * sizeof (unsigned short**));
    for (i = 0; i < image->pxHeight; i++) {
        image->pxArr[i] = (unsigned short**) malloc(image->pxWidth * sizeof (unsigned short*));
        for (j = 0; j < image->pxWidth; j++) {
            image->pxArr[i][j] = (unsigned short*) malloc(4 * sizeof (unsigned short));
        }
    }

    assert(dibHSize >= DIB_INFO);
    if (dibHSize == DIB_INFO) {
        unsigned rowIndex = image->pxHeight - 1;
        for (i = offsetPxArr; i < endPxArr; i += lineSize, rowIndex--) {
            unsigned colIndex = 0;
            for (j = i; j < i + lineSize - paddingPLine; j += bytesPPx, colIndex++) {
                unsigned pixel = get_bytes(j, j + bytesPPx, image->fBuffer);
                image->pxArr[rowIndex][colIndex][0] = colorTable[pixel][0];
                image->pxArr[rowIndex][colIndex][1] = colorTable[pixel][1];
                image->pxArr[rowIndex][colIndex][2] = colorTable[pixel][2];
                image->pxArr[rowIndex][colIndex][3] = colorTable[pixel][3];
            }
            assert(colIndex == image->pxWidth);
        }
        assert(rowIndex == 4294967295);
        return;
    }

    assert(bitsPPx == BMP_16BPP || bitsPPx == BMP_24BPP || bitsPPx == BMP_32BPP);

    assert(dibHSize >= DIB_V4);
    abort_("We don't know what to do with DIB_V4 yet file: %s", image->fName);
    if (dibHSize == DIB_V4) {
        return;
    }

    assert(dibHSize == DIB_V5);
    abort_("We don't know what to do with DIB_V5 yet file: %s", image->fName);
    return;
}

void write_bmp(UC_IMAGE* image, const char* fileName){
    char bitmapStream[1000];
    unsigned i;
    
    unsigned fileSize = BMP_HEADER_SIZE;
    unsigned offsetPxArr;
    
    // -- FILE HEADER -- //

    // bitmapStream signature
    bitmapStream[0] = 'B';
    bitmapStream[1] = 'M';

    // file fileSize
    bitmapStream[2] = fileSize & 0x000000FF;
    bitmapStream[3] = fileSize & 0x0000FF00;
    bitmapStream[4] = fileSize & 0x00FF0000;
    bitmapStream[5] = fileSize & 0xFF000000;

    // reserved field (in hex. 00 00 00 00)
    for(i = 6; i < 10; i++) bitmapStream[i] = 0;

    // offset of pixel data inside the image
    bitmapStream[10] = offsetPxArr & 0x000000FF;
    bitmapStream[11] = offsetPxArr & 0x0000FF00;
    bitmapStream[12] = offsetPxArr & 0x00FF0000;
    bitmapStream[13] = offsetPxArr & 0xFF000000;

    // -- BITMAP HEADER -- //

    // header fileSize
    bitmapStream[14] = 40;
    for(i = 15; i < 18; i++) bitmapStream[i] = 0;

    // width of the image
    bitmapStream[18] = 4;
    for(i = 19; i < 22; i++) bitmapStream[i] = 0;

    // height of the image
    bitmapStream[22] = 1;
    for(i = 23; i < 26; i++) bitmapStream[i] = 0;

    // reserved field
    bitmapStream[26] = 1;
    bitmapStream[27] = 0;

    // number of bits per pixel
    bitmapStream[28] = 24; // 3 byte
    bitmapStream[29] = 0;

    // compression method (no compression here)
    for(i = 30; i < 34; i++) bitmapStream[i] = 0;

    // fileSize of pixel data
    bitmapStream[34] = 12; // 12 bits => 4 pixels
    bitmapStream[35] = 0;
    bitmapStream[36] = 0;
    bitmapStream[37] = 0;

    // horizontal resolution of the image - pixels per meter (2835)
    bitmapStream[38] = 0;
    bitmapStream[39] = 0;
    bitmapStream[40] = 0b00110000;
    bitmapStream[41] = 0b10110001;

    // vertical resolution of the image - pixels per meter (2835)
    bitmapStream[42] = 0;
    bitmapStream[43] = 0;
    bitmapStream[44] = 0b00110000;
    bitmapStream[45] = 0b10110001;

    // color pallette information
    for(i = 46; i < 50; i++) bitmapStream[i] = 0;

    // number of important colors
    for(i = 50; i < 54; i++) bitmapStream[i] = 0;

    // -- PIXEL DATA -- //
    for(i = 54; i < 66; i++) bitmapStream[i] = 0;

    FILE *file;
    file = fopen(fileName, "wb");
    fputs(bitmapStream, file);
    fclose(file);
}
