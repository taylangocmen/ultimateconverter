
/* 
 * File:   uUtils.h
 * Author: gocmenta
 *
 * Created on March 14, 2017, 1:32 AM
 */

#ifndef UUTILS_H
#define UUTILS_H

enum format {
  bmp,
  jpg,
  jpeg,
  png,
};

unsigned get_bytes(unsigned i, unsigned j, unsigned char* buffer);

#endif /* UUTILS_H */

