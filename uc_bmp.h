
#ifndef UC_BMP_H
#define UC_BMP_H

#include "uc_utils.h"
#include "uc_image.h"

#define BMP_HEADER_SIZE 14

#define BMP_BM 'BM'

#define DIB_CORE 12
#define DIB_INFO 40
#define DIB_V4 108
#define DIB_V5 124

#define BMP_1BPP 1
#define BMP_2BPP 2
#define BMP_4BPP 4
#define BMP_8BPP 8
#define BMP_16BPP 16
#define BMP_24BPP 24
#define BMP_32BPP 32

void read_bmp(UC_IMAGE* image);

#endif /* UC_BMP_H */
