
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "uc_jpg.h"
#include "uc_image.h"
#include "uc_utils.h"

void read_jpg(UC_IMAGE* image){
    unsigned i, byte;
    for(i = 0; i < image->fSize; i++){
        byte = get_bytes(i, i+1, image->fBuffer);
        printf("%u _ ", i);
        print_unsigned_bytes(byte, 1);
        if(byte == JPG_MRK){
            
            
        }
    }
    
}

void write_jpg(UC_IMAGE* image, const char* fileName){
    
    
}
