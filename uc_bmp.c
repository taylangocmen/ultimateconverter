
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "uc_bmp.h"
#include "uc_image.h"
#include "uc_utils.h"

void read_bmp(UC_IMAGE* image) {
    unsigned i, j;

    unsigned signature = get_bytes(0, 2, image->fBuffer);
    if (signature != BMP_BM)
        abort_("File was to be a BMP but was not.");

    unsigned offsetPxArr = get_bytes(14, 10, image->fBuffer);
    unsigned dibHSize = get_bytes(18, 14, image->fBuffer);
    image->pxWidth = get_bytes(22, 18, image->fBuffer);
    image->pxHeight = get_bytes(26, 22, image->fBuffer);

    if (dibHSize != DIB_CORE && dibHSize != DIB_INFO && dibHSize != DIB_V4 && dibHSize != DIB_V5)
        abort_("File has an unsupported DIBHEADER size: %u ", dibHSize);

    assert(dibHSize >= DIB_CORE);
    if (dibHSize == DIB_CORE) {
        abort_("We don't know what to do with DIB_CORE yet file.");
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
        abort_("We don't know what to do with this BPP yet file.");
    } else abort_("File has an unsupported BPP");


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
    abort_("We don't know what to do with DIB_V4 yet file.");
    if (dibHSize == DIB_V4) {
        return;
    }

    assert(dibHSize == DIB_V5);
    abort_("We don't know what to do with DIB_V5 yet file");
    return;
}

unsigned write_bmp(UC_IMAGE* image, volatile int *toAddr){
    unsigned pxWidth = image->pxWidth;
    unsigned pxHeight = image->pxHeight;
    unsigned pxFormat = image->pxFormat;
    assert(pxFormat == 3);

    unsigned offsetPxArr = BMP_HEADER_SIZE + DIB_INFO + (4 * BMP_256);
    unsigned padding = (4 - (pxWidth % 4));
    if(padding == 4) padding = 0;
    unsigned lineSize = (pxWidth + padding);
    unsigned imageSize = (pxHeight * lineSize);
    unsigned fileSize = offsetPxArr + imageSize;
//    unsigned char* bitmapStream = (unsigned char *)malloc(fileSize * sizeof(unsigned char));
    unsigned char* bitmapStream = (unsigned char *)toAddr;
    unsigned i, j;
    
    // bitmapStream signature
    bitmapStream[0] = 'B';
    bitmapStream[1] = 'M';
    
    // file fileSize
    bitmapStream[2] = BOFF0(fileSize);
    bitmapStream[3] = BOFF1(fileSize);
    bitmapStream[4] = BOFF2(fileSize);
    bitmapStream[5] = BOFF3(fileSize);
    
    // reserved field (in hex. 00 00 00 00)
    for(i = 6; i < 10; i++) bitmapStream[i] = 0;
    
    // offset of pixel data inside the image
    bitmapStream[10] = BOFF0(offsetPxArr);
    bitmapStream[11] = BOFF1(offsetPxArr);
    bitmapStream[12] = BOFF2(offsetPxArr);
    bitmapStream[13] = BOFF3(offsetPxArr);

    // -- BITMAP HEADER -- // // header fileSize
    bitmapStream[14] = BOFF0(DIB_INFO);
    bitmapStream[15] = BOFF1(DIB_INFO);
    bitmapStream[16] = BOFF2(DIB_INFO);
    bitmapStream[17] = BOFF3(DIB_INFO);

    // width of the image
    bitmapStream[18] = BOFF0(pxWidth);
    bitmapStream[19] = BOFF1(pxWidth);
    bitmapStream[20] = BOFF2(pxWidth);
    bitmapStream[21] = BOFF3(pxWidth);

    // height of the image
    bitmapStream[22] = BOFF0(pxHeight);
    bitmapStream[23] = BOFF1(pxHeight);
    bitmapStream[24] = BOFF2(pxHeight);
    bitmapStream[25] = BOFF3(pxHeight);

    // Planes
    bitmapStream[26] = 1;
    bitmapStream[27] = 0;

    // number of bits per pixel
    bitmapStream[28] = 8; // 3 byte
    bitmapStream[29] = 0;

    // compression method (no compression)
    for(i = 30; i < 34; i++) bitmapStream[i] = 0;

    // fileSize of pixel data
    bitmapStream[34] = BOFF0(imageSize);
    bitmapStream[35] = BOFF1(imageSize);
    bitmapStream[36] = BOFF2(imageSize);
    bitmapStream[37] = BOFF3(imageSize);

    // horizontal resolution of the image - pixels per meter (2835)
    bitmapStream[38] = BOFF0(BMP_PPM);
    bitmapStream[39] = BOFF1(BMP_PPM);
    bitmapStream[40] = BOFF2(BMP_PPM);
    bitmapStream[41] = BOFF3(BMP_PPM);

    // vertical resolution of the image - pixels per meter (2835)
    bitmapStream[42] = BOFF0(BMP_PPM);
    bitmapStream[43] = BOFF1(BMP_PPM);
    bitmapStream[44] = BOFF2(BMP_PPM);
    bitmapStream[45] = BOFF3(BMP_PPM);

    // color pallette information
    bitmapStream[46] = BOFF0(BMP_256);
    bitmapStream[47] = BOFF1(BMP_256);
    bitmapStream[48] = BOFF2(BMP_256);
    bitmapStream[49] = BOFF3(BMP_256);

    // number of important colors
    for(i = 50; i < 54; i++) bitmapStream[i] = 0;

    // -- color table -- //
    for(i = 54, j = 0; j < BMP_256; i+=4, j++){
        unsigned color = bmp256_color_table_index_to_color(j);
        bitmapStream[i] = BOFF0(color);
        bitmapStream[i+1] = BOFF1(color);
        bitmapStream[i+2] = BOFF2(color);
        bitmapStream[i+3] = BOFF3(color);
    }

    unsigned a, b;
    
    for(a = pxHeight; a > 0; a--){
        for(b = 0; b < pxWidth; b++, i++) {
            unsigned short r = image->pxArr[a-1][b][0];
            unsigned short g = image->pxArr[a-1][b][1];
            unsigned short b = image->pxArr[a-1][b][2];
            unsigned index = bmp256_color_table_color_to_index(r, g, b);
            bitmapStream[i] = index;
        }
        for(b = 0; b < padding; b++, i++)
            bitmapStream[i] = 0;
    }
    
//    FILE *file;
//    file = fopen(fileName, "wb");
//    for(i = 0; i < fileSize; i++)
//        fputc(bitmapStream[i], file);
//    fclose(file);
//    free(bitmapStream);
    return fileSize;
}

#define FACTBLUE 6
#define FACTGREEN 6
#define FACTRED 6

unsigned bmp256_color_table_index_to_color(unsigned i){
    assert(i >= 0);
    assert(i < BMP_256);

    unsigned blue = i % FACTBLUE;
    unsigned green = (i / FACTBLUE) % FACTGREEN;
    unsigned red = (i / (FACTBLUE * FACTGREEN)) % FACTRED;
    
    unsigned color = 0;
    
    color += round_((float)red * ((float)255 / (float)(FACTBLUE-1)));
    color *= BMP_256;

    color += round_((float)green * ((float)255 / (float)(FACTGREEN-1)));
    color *= BMP_256;
    
    color += round_((float)blue * ((float)255 / (float)(FACTRED-1)));
    return color;
}

unsigned bmp256_color_table_color_to_index(unsigned short r, unsigned short g, unsigned short b){
    assert(r >= 0);
    assert(r < BMP_256);
    assert(g >= 0);
    assert(g < BMP_256);
    assert(b >= 0);
    assert(b < BMP_256);
    
    unsigned red = round_((float)r / ((float)255 / (float)(FACTRED-1)));
    unsigned green = round_((float)g / ((float)255 / (float)(FACTGREEN-1)));
    unsigned blue = round_((float)b / ((float)255 / (float)(FACTBLUE-1)));
    
    assert(red >= 0 && red < FACTRED);
    assert(green >= 0 && green < FACTGREEN);
    assert(blue >= 0 && blue < FACTBLUE);
    
    unsigned index = (red*(FACTBLUE * FACTGREEN)) + (green*FACTBLUE) + blue;
    return index;
}