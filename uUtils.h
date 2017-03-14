
/* 
 * File:   uUtils.h
 * Author: gocmenta
 *
 * Created on March 14, 2017, 1:32 AM
 */

#ifndef UUTILS_H
#define UUTILS_H

#define BINARY(x)    (print_binary(sizeof(x), &x))

enum format {
  bmp,
  jpg,
  png,
};

unsigned to_power(unsigned base, unsigned power);
void print_binary(size_t const size, void const* const ptr);

#endif /* UUTILS_H */

