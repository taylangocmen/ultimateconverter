
/* 
 * File:   png.cpp
 * Author: gocmenta
 *
 * Created on March 2, 2017, 1:21 AM
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <cstring>
#include "zlib.h"

#include "utils.h"
#include "png.h"
#include "zpipe.h"

using namespace std;

void print_png(uImage* img) {
  unsigned char* buffer = img->buffer;
  long lSize = img->lSize;
  unsigned char* chunkdataHex;

  cout << "----------------------------------------------" << endl;
  cout << "PNG Header" << endl;
  long transmissionSystem = get_bits(0, 1, buffer);
  assert(transmissionSystem = PNG_8bit);
//  print_bits(0, 1, buffer); cout << " - ";
//  cout << "transmissionSystem: " << transmissionSystem << endl;
  long signature = get_bits(1, 4, buffer);
  assert(signature == PNG_PNG);
//  print_bits(1, 4, buffer); cout << " - ";
//  cout << "signature: " << signature << endl;
  long crlf = get_bits(4, 6, buffer);
  assert(crlf == PNG_CRLF);
//  print_bits(4, 6, buffer); cout << " - ";
//  cout << "crlf: " << crlf << endl;
  long endoffile = get_bits(6, 7, buffer);
  assert(endoffile == PNG_SUB);
//  print_bits(6, 7, buffer); cout << " - ";
//  cout << "endoffile: " << endoffile << endl;
  long lf = get_bits(7, 8, buffer);
  assert(lf == PNG_LF);
//  print_bits(7, 8, buffer); cout << " - ";
//  cout << "lf: " << lf << endl;
  cout << "Headers are all OK" << endl;
  cout << "----------------------------------------------" << endl;

  long chunkIndex = PNG_HEADER_SIZE;

  while (chunkIndex < lSize) {
    long length = get_bits(chunkIndex, chunkIndex + 4, buffer);
    chunkIndex += 4;

    long chunktype = get_bits(chunkIndex, chunkIndex + 4, buffer);
    bool critical = check_png_header_crit(chunktype);
    chunkIndex += 4;


    if (critical) {
      unsigned long long chunkdata = get_ll_bits(chunkIndex, chunkIndex + length, buffer);

      cout << "length: " << length << endl;
      cout << "critical: ";
      critical ? cout << "true" : cout << "false";
      cout << endl;
      cout << "chunktype: " << header_to_string(chunktype) << endl;
      cout << "chunkdata: " << chunkdata << endl;
      if (chunktype == PNG_IHDR) {
        long width = get_bits(chunkIndex, chunkIndex + 4, buffer);
        cout << "width: " << width << endl;
        long height = get_bits(chunkIndex + 4, chunkIndex + 8, buffer);
        cout << "height: " << height << endl;
        long bitDepth = get_bits(chunkIndex + 8, chunkIndex + 9, buffer);
        cout << "bitDepth: " << bitDepth << endl;
        long colorType = get_bits(chunkIndex + 9, chunkIndex + 10, buffer);
        cout << "colorType: " << colorType << endl;
        long compressionMethod = get_bits(chunkIndex + 10, chunkIndex + 11, buffer);
        cout << "compressionMethod: " << compressionMethod << endl;
        long filterMethod = get_bits(chunkIndex + 11, chunkIndex + 12, buffer);
        cout << "filterMethod: " << filterMethod << endl;
        long interlaceMethod = get_bits(chunkIndex + 12, chunkIndex + 13, buffer);
        cout << "interlaceMethod: " << interlaceMethod << endl;

      } else if (chunktype == PNG_IDAT) {
        chunkdataHex = get_bits_hex(chunkIndex, chunkIndex + length, buffer);
        cout << "chunkdataHex: " << chunkdataHex << endl;
        
        uLong ucompSize = 40000;
        uLong compSize = length-8+1;


        
        
        char comp[length-8];
        char reComp[length-8];
        for(long x = 0; x < length-8; x++)
          comp[x] = (char)chunkdataHex[x+4];
        char unComp[ucompSize];
        
        for(long x = 0; x < 2; x++)
          printf("comp[%6d]:   %02X\n", x, (unsigned char)comp[x]);
        
        for(long x = length-4; x < length; x++)
          printf("comp[%6d]:   %02X\n", x, (unsigned char)comp[x]);
        
        
        cout << "ucompSize: " << ucompSize << endl;
        cout << "compSize: " << compSize << endl;

        cout << "unComp: " << unComp << endl;
        cout << "comp: " << comp << endl << endl;

        uncompress((Bytef *)unComp, &ucompSize, (Bytef *)comp, compSize);// Inflate
        compress((Bytef *)reComp, &compSize, (Bytef *)unComp, ucompSize);         // Deflate
        
        cout << "comp: " << comp << endl;
        cout << "unComp: " << unComp << endl;
        cout << "reComp: " << comp << endl << endl;
        
//        for(long x = 0; x < ucompSize; x++)
        for(long y = 0; y < 200; y++){
          for(long x = 0; x < 200; x++)
            printf("%02X", (unsigned char)unComp[x+200*y]);
          cout << endl;
        }

//        print_bits(chunkIndex, chunkIndex + length, buffer);
      }
    }
//    else {
//      cout << "chunkdata: " << "not critical";
//    }
    
    chunkIndex += length;

    long crc = get_bits(chunkIndex, chunkIndex + 4, buffer);
    
    if (critical) cout << "crc: " << crc << endl;
    chunkIndex += 4;
    
    cout << "----------------------------------------------" << endl;
  }

  delete [] chunkdataHex;
}

bool check_png_header_crit(long header) {
  return
  header == PNG_IHDR ||
    header == PNG_PLTE ||
    header == PNG_IDAT ||
    header == PNG_IEND;
}

const char* header_to_string(long header) {
  switch (header) {
    case PNG_IHDR:
      return "IHDR";
    case PNG_PLTE:
      return "PLTE";
    case PNG_IDAT:
      return "IDAT";
    case PNG_IEND:
      return "IEND";
    case PNG_bKGD:
      return "bKGD";
    case PNG_cHRM:
      return "cHRM";
    case PNG_gAMA:
      return "gAMA";
    case PNG_hIST:
      return "hIST";
    case PNG_iCCP:
      return "iCCP";
    case PNG_iTXt:
      return "iTXt";
    case PNG_pHYs:
      return "pHYs";
    case PNG_sBIT:
      return "sBIT";
    case PNG_sPLT:
      return "sPLT";
    case PNG_sRGB:
      return "sRGB";
    case PNG_sTER:
      return "sTER";
    case PNG_tEXt:
      return "tEXt";
    case PNG_tIME:
      return "tIME";
    case PNG_tRNS:
      return "tRNS";
    case PNG_zTXt:
      return "zTXt";
    default:
      return "UNDEFINED HEADER";
  }
}
