
/* 
 * File:   uUtils.cpp
 * Author: gocmenta
 *
 * Created on March 14, 2017, 1:32 AM
 */

unsigned long get_bytes(unsigned long i, unsigned long j, unsigned char* buffer){
  unsigned long sum = 0;
  
  if (j > i) {
    
    for (long k = i; k < j; k++) {
      sum *= 256;
      sum += (long) buffer[k];
    }
  } else if(i < j) {
    for (long k = i - 1; k > j - 1; k--) {
      sum *= 256;
      sum += (long) buffer[k];
    }
  }
  return sum;
}
