
/* 
 * File:   zpipe.h
 * Author: gocmenta
 *
 * Created on March 2, 2017, 8:06 AM
 */

#ifndef ZPIPE_H
#define ZPIPE_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

int def(FILE *source, FILE *dest, int level);
int inf(FILE *source, FILE *dest);
void zerr(int ret);
int zpipe_main(int argc, char **argv);

#endif /* ZPIPE_H */

