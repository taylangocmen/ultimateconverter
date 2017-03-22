
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

void write_bmp_oldversion(UC_IMAGE* image, const char* fileName){
    
    printf("%s\n", image->fName);
    printf("to\n");
    printf("%s\n\n", fileName);
    
    unsigned signature = get_bytes(0, 2, image->fBuffer);
    printf("signature:\n");
    print_unsigned_bytes(signature, 2);
    
    unsigned offsetPxArr = get_bytes(14, 10, image->fBuffer);
    printf("\noffsetPxArr:\n");
    print_unsigned_bytes(offsetPxArr, 4);
    
    unsigned dibHSize = get_bytes(18, 14, image->fBuffer);
    printf("\ndibHSize:\n");
    print_unsigned_bytes(dibHSize, 4);
    
//    image->pxWidth = get_bytes(22, 18, image->fBuffer);
    printf("\nimage->pxWidth:\n");
    print_unsigned_bytes(image->pxWidth, 4);
    
//    image->pxHeight = get_bytes(26, 22, image->fBuffer);
    printf("\nimage->pxHeight:\n");
    print_unsigned_bytes(image->pxHeight, 4);
    
    unsigned bitsPPx = get_bytes(30, 28, image->fBuffer);
    printf("\nbitsPPx:\n");
    print_unsigned_bytes(bitsPPx, 2);
    
    unsigned imageSize = get_bytes(38, 34, image->fBuffer);
    printf("\nimageSize:\n");
    print_unsigned_bytes(imageSize, 4);
    
    unsigned colorsInTable = get_bytes(50, 46, image->fBuffer);
    printf("\ncolorsInTable:\n");
    print_unsigned_bytes(colorsInTable, 4);
    
    unsigned endPxArr = offsetPxArr + imageSize;
    printf("\nendPxArr:\n");
    print_unsigned_bytes(endPxArr, 4);
    
    unsigned bytesPPx = bitsPPx / 8;
    printf("\nbytesPPx:\n");
    print_unsigned_bytes(bytesPPx, 2);
    
    unsigned lineSize = imageSize / image->pxHeight;
    printf("\nlineSize:\n");
    print_unsigned_bytes(lineSize, 4);
    
    unsigned paddingPLine = lineSize % bytesPPx;
    printf("\npaddingPLine:\n");
    print_unsigned_bytes(paddingPLine, 1);
    
    assert(lineSize - paddingPLine == image->pxWidth * bytesPPx);

    
    
    
//    unsigned char bitmapStream[1000];
//    
//    // bitmapStream signature
//    unsigned signature = get_bytes(0, 2, image->fBuffer);
//    bitmapStream[0] = BOFF0(signature);
//    bitmapStream[1] = BOFF1(signature);
//    
//    // file fileSize
//    bitmapStream[2] = ;
//    bitmapStream[3] = ;
//    bitmapStream[4] = ;
//    bitmapStream[5] = ;
//    
//    // reserved field (in hex. 00 00 00 00)
//    for(i = 6; i < 10; i++) 
//        bitmapStream[i] = ;
//    
//    // offset of pixel data inside the image
//    bitmapStream[10] = ;
//    bitmapStream[11] = ;
//    bitmapStream[12] = ;
//    bitmapStream[13] = ;
//
//    // -- BITMAP HEADER -- //
//    // header fileSize
//    bitmapStream[14] = ;
//    bitmapStream[15] = ;
//    bitmapStream[16] = ;
//    bitmapStream[17] = ;
//
//    // width of the image
//    bitmapStream[18] = ;
//    bitmapStream[19] = ;
//    bitmapStream[20] = ;
//    bitmapStream[21] = ;
//
//    // height of the image
//    bitmapStream[22] = ;
//    bitmapStream[23] = ;
//    bitmapStream[24] = ;
//    bitmapStream[25] = ;
//
//
//    // Planes
//    bitmapStream[26] = ;
//    bitmapStream[27] = ;
//
//    // number of bits per pixel
//    bitmapStream[28] = ; // 3 byte
//    bitmapStream[29] = ;
//
//    // compression method (no compression)
//    for(i = 30; i < 34; i++)
//        bitmapStream[i] = ;
//
//    // fileSize of pixel data
//    bitmapStream[34] = ;
//    bitmapStream[35] = ;
//    bitmapStream[36] = ;
//    bitmapStream[37] = ;
//
//    // horizontal resolution of the image - pixels per meter (2835)
//    bitmapStream[38] = ;
//    bitmapStream[39] = ;
//    bitmapStream[40] = ;
//    bitmapStream[41] = ;
//
//    // vertical resolution of the image - pixels per meter (2835)
//    bitmapStream[42] = ;
//    bitmapStream[43] = ;
//    bitmapStream[44] = ;
//    bitmapStream[45] = ;
//
//    // color pallette information
//    for(i = 46; i < 50; i++)
//        bitmapStream[i] = ;
//
//    // number of important colors
//    for(i = 50; i < 54; i++)
//        bitmapStream[i] = ;
//
//    // -- PIXEL DATA -- //
//    for(i = 54; i < 66; i++)
//        bitmapStream[i] = ;
//
//    FILE *file;
//    file = fopen(fileName, "wb");
//    fputs(bitmapStream, file);
//    fclose(file);
}


void write_bmp(UC_IMAGE* image, const char* fileName){
    
//    printf("BOFF0(BMP_PPM): %02X\n", BOFF0(BMP_PPM));
//    printf("BOFF1(BMP_PPM): %02X\n", BOFF1(BMP_PPM));
//    printf("BOFF2(BMP_PPM): %02X\n", BOFF2(BMP_PPM));
//    printf("BOFF3(BMP_PPM): %02X\n", BOFF3(BMP_PPM));
    
//    printf("%f = %u\n", BMP_BLUEINDEX256(BMP_BLUE256(0)), round_(BMP_BLUEINDEX256(BMP_BLUE256(0))));
//    printf("%f = %u\n", BMP_BLUEINDEX256(BMP_BLUE256(1)), round_(BMP_BLUEINDEX256(BMP_BLUE256(1))));
//    printf("%f = %u\n", BMP_BLUEINDEX256(BMP_BLUE256(2)), round_(BMP_BLUEINDEX256(BMP_BLUE256(2))));
//    printf("%f = %u\n", BMP_BLUEINDEX256(BMP_BLUE256(3)), round_(BMP_BLUEINDEX256(BMP_BLUE256(3))));
    
    unsigned pxWidth = image->pxWidth;
    unsigned pxHeight = image->pxHeight;
    unsigned pxFormat = image->pxFormat;
    assert(pxFormat == 3);
   
//    unsigned char bitmapStream[1000];
//    for(i = 0; i < 256; i++){
//        unsigned color = bmp256_color_table_index_to_color(i);
//        printf("i=%u : color=%u : index=", i, color);
//        fflush(stdout);
//        unsigned index = bmp256_color_table_color_to_index(BOFF2(color), BOFF1(color), BOFF0(color));
////        printf("i=%u : BOFF0(color)=%02X\n", i, BOFF0(color));
////        printf("i=%u : BOFF1(color)=%02X\n", i, BOFF1(color));
////        printf("i=%u : BOFF2(color)=%02X\n", i, BOFF2(color));
////        printf("i=%u : BOFF3(color)=%02X\n", i, BOFF3(color));
//        printf("%u\n", index);
//        fflush(stdout);
//    }
    
    
    unsigned offsetPxArr = BMP_HEADER_SIZE + DIB_INFO + (4 * BMP_256);
    unsigned padding = (4 - (pxWidth % 4));
    if(padding == 4) padding = 0;
    unsigned lineSize = (pxWidth + padding);
    unsigned imageSize = (pxHeight * lineSize);
    unsigned fileSize = offsetPxArr + imageSize;
    unsigned char* bitmapStream = (unsigned char *)malloc(fileSize * sizeof(unsigned char));
    unsigned i, j;
    
    
//    printf("BOFF0(fileSize): %02X\n", BOFF0(fileSize));
//    printf("BOFF1(fileSize): %02X\n", BOFF1(fileSize));
//    printf("BOFF2(fileSize): %02X\n", BOFF2(fileSize));
//    printf("BOFF3(fileSize): %02X\n", BOFF3(fileSize));
    
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
//    printf("pxFormat:%u\n", image->pxFormat);
//    if(image->pxFormat == 3)
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
//        printf("%u = %u\n", j, bmp256_color_table(j));
        unsigned color = bmp256_color_table_index_to_color(j);
        bitmapStream[i] = BOFF0(color);
        bitmapStream[i+1] = BOFF1(color);
        bitmapStream[i+2] = BOFF2(color);
        bitmapStream[i+3] = BOFF3(color);
//        printf("%u - BOFF0(color): %02X\n", j, BOFF0(color));
//        printf("%u - BOFF1(color): %02X\n", j, BOFF1(color));
//        printf("%u - BOFF2(color): %02X\n", j, BOFF2(color));
//        printf("%u - BOFF3(color): %02X\n", j, BOFF3(color));

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
        for(b = 0; b < padding; b++, i++){
            bitmapStream[i] = 0;
        }
    }
    
//    for(i = 0; i < 1000; i++){
//        printf("\n%u=%02X  ---  %C\n", i, i, bitmapStream[i]);
//        print_unsigned_bytes(bitmapStream[i], 1);
//    }
//    printf("Test");
    
    FILE *file;
    file = fopen(fileName, "wb");
//    fputs((char*)bitmapStream, file);
    for(i = 0; i < fileSize; i++)
        fputc(bitmapStream[i], file);
    
    fclose(file);
    
    free(bitmapStream);
}

unsigned bmp256_color_table_index_to_color(unsigned i){
    assert(i >= 0);
    assert(i < BMP_256);
    
    unsigned red = BMP_RED256(i);
    unsigned color = red;
    color *= BMP_256;
    
    unsigned green = BMP_GREEN256(i);
    color += green;
    color *= BMP_256;
    
    unsigned blue = BMP_BLUE256(i);
    color += blue;
    
    return color;
}

unsigned bmp256_color_table_color_to_index(unsigned short r, unsigned short g, unsigned short b){
    assert(r >= 0);
    assert(r < BMP_256);
    assert(g >= 0);
    assert(g < BMP_256);
    assert(b >= 0);
    assert(b < BMP_256);
    
    unsigned red = round_(BMP_REDINDEX256(r));
    unsigned green = round_(BMP_GREENINDEX256(g));
    unsigned blue = round_(BMP_BLUEINDEX256(b));
    
    unsigned index = (red << 5) + (green << 2) + blue;
    
    return index;
}