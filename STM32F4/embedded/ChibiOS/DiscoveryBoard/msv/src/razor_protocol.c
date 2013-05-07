#include <stdlib.h>
#include <stdio.h>
#include "msv/include/razor_protocol.h"

void razorToBits(int razorData[12], unsigned char* byte){
  byte[0] = razorData[0];
  byte[1] = razorData[0] >> 8;
  byte[1] |= razorData[3] << 1;
  byte[2] = razorData[3] >> 7;
  byte[2] |= razorData[4] << 2;
  byte[3] = razorData[4] >> 6;
  byte[3] |= razorData[6] << 3;
  byte[4] = razorData[6] >> 5;
  byte[4] |= razorData[7] << 4;
  byte[5] = razorData[7] >> 4;
  byte[5] |= razorData[9] << 5;
  byte[6] = razorData[9] >> 3;
  byte[6] |= razorData[10] << 6;
  byte[7] = razorData[10] >> 2;
}
