
/* 
 * File:   png.h
 * Author: gocmenta
 *
 * Created on March 2, 2017, 1:07 AM
 */

#ifndef PNG_H
#define PNG_H

// A PNG header is always 8 bytes
#define PNG_HEADER_SIZE 8

// The following are always present at the header of a PNG image
#define PNG_8bit 137
#define PNG_PNG 'PNG'
#define PNG_CRLF 3338
#define PNG_SUB 26
#define PNG_LF 10

#define PNG_IHDR 'IHDR'
#define PNG_PLTE 'PLTE'
#define PNG_IDAT 'IDAT'
#define PNG_IEND 'IEND'

#define PNG_bKGD 'bKGD'
#define PNG_cHRM 'cHRM'
#define PNG_gAMA 'gAMA'
#define PNG_hIST 'hIST'
#define PNG_iCCP 'iCCP'
#define PNG_iTXt 'iTXt'
#define PNG_pHYs 'pHYs'
#define PNG_sBIT 'sBIT'
#define PNG_sPLT 'sPLT'
#define PNG_sRGB 'sRGB'
#define PNG_sTER 'sTER'
#define PNG_tEXt 'tEXt'
#define PNG_tIME 'tIME'
#define PNG_tRNS 'tRNS'
#define PNG_zTXt 'zTXt'


void print_png(uFile* img);
bool check_png_header_crit(long header);
const char* header_to_string(long header);

#endif /* PNG_H */

