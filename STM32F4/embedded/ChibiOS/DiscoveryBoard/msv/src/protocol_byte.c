#include <stdlib.h>
#include <stdio.h>
#include "msv/include/protocol_byte.h"
#include <string.h>
#include <stdint.h>

#define TERM 0x01


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

uint8_t ultrasonic(int value){
  
  uint8_t byte;
  if(value <= 63){
    byte = value;
  }else if(value <= 189){
    byte = value / 3;
    byte |= 1 << 6;
  }else if(value <= 315){
    byte = value / 5;
    byte |= 1 << 7;
  }else if(value <= 630){
    byte = value / 10;
    byte |= 1 << 6;
    byte |= 1 << 7;
  }else{
    byte = 0;
  }
  
  return byte;
}
uint32_t infrared(int ir1, int ir2, int ir3){
  uint32_t byte = 0;
  byte |= ir1;
  byte |= ir2 << 5;
  byte |= ir3 << 10;
  return byte;
}

uint32_t translate(uint16_t receive){
  uint16_t request;
  uint32_t package = 0;

  request = receive & 0x000F;
  package |= request;

  switch(request){
  case 1:
    package |= infrared(16,8,7) << 4;
    break;
  case 2:
    package |= ultrasonic(533) << 4;
    package |= ultrasonic(233) << 12;
    package |= ultrasonic(533) << 20;
    break;
  }

  package |= TERM << 30;  
  printf("YOOOO   %x",package);

  return package;
}
