
#ifndef UC_JPG_H
#define UC_JPG_H

#include "uc_utils.h"
#include "uc_image.h"

#define JPG_MRK 255

#define JPG_MRK_SOI 0xD8
#define JPG_MRK_SOF0 0xC0
#define JPG_MRK_SOF2 0xC2
#define JPG_MRK_DHT 0xC4
#define JPG_MRK_DQT 0xDB
#define JPG_MRK_DRI 0xDD
#define JPG_MRK_SOS 0xDA

#define JPG_MRK_RST(p) (((p>>4)==0xD) && ((p & 0xF) >= 0x0) && ((p & 0xF) <= 0x7))
#define JPG_MRK_RST0 0xD0
#define JPG_MRK_RST1 0xD1
#define JPG_MRK_RST2 0xD2
#define JPG_MRK_RST3 0xD3
#define JPG_MRK_RST4 0xD4
#define JPG_MRK_RST5 0xD5
#define JPG_MRK_RST6 0xD6
#define JPG_MRK_RST7 0xD7

#define JPG_MRK_APP(p) (((p>>4)==0xE) && ((p & 0xF) >= 0x0) && ((p & 0xF) <= 0x7))
#define JPG_MRK_APP0 0xE0
#define JPG_MRK_APP1 0xE1
#define JPG_MRK_APP2 0xE2
#define JPG_MRK_APP3 0xE3
#define JPG_MRK_APP4 0xE4
#define JPG_MRK_APP5 0xE5
#define JPG_MRK_APP6 0xE6
#define JPG_MRK_APP7 0xE7

#define JPG_MRK_COM 0xFE
#define JPG_MRK_EOI 0xD9

void read_jpg(UC_IMAGE* image);

void write_jpg(UC_IMAGE* image, const char* fileName);

#endif /* UC_JPG_H */

