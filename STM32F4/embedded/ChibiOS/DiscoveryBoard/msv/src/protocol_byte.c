#include <stdlib.h>
#include <stdio.h>
#include "msv/include/protocol_byte.h"
#include <string.h>
#include <stdint.h>

#define TERM 0x01

/***************************** RAZOR DATA MAP *******************************
 *                [0] = Yaw    [1] = Pitch   [2] = Roll 
 * Magnetometer:  [3] = X      [4] = Y       [5] = Z
 * Gyroscope:     [6] = X      [7] = Y       [8] = Z
 * Accelerometer: [9] = X     [10] = Y      [11] = Z
 */


uint32_t yaw(int razorData[12]){
  uint32_t encoded = 0;
  if(razorData[0] < -180 || razorData[0] > 180){
    encoded |= 0x0100;
    return encoded;
  }
  
  if(razorData[0] < 0){
    encoded |= 0x0100;
  }
  
  encoded |= razorData[0];
  
  return encoded;
}
uint32_t magnetometer(int razorData[12]){
  uint32_t encoded = 0;
  uint8_t flag = 0;
  if(razorData[3] < -250 || razorData[3] > 250){
    encoded |= 0x0100;
    flag = 1;
  }
  if(razorData[4] < -250 || razorData[4] > 250){
    encoded |= 0x0100 << 9;
    flag = 1;
  }

  if(flag == 1){
    return encoded;
  }
  
  if(razorData[3] < 0){
    encoded |= 0x0100;
  }
  if(razorData[4] < 0){
    encoded |= 0x0100 << 9;
  }
  
  encoded |= razorData[3];
  encoded |= razorData[4] << 9;
  //encoded |= yaw(razorData) << 18;
  return encoded;
}

uint32_t gyroscope(int razorData[12]){
  uint32_t encoded = 0;
  uint8_t flag = 0;
  if(razorData[6] < -250 || razorData[6] > 250){
    encoded |= 0x0100;
    flag = 1;
  }
  if(razorData[7] < -250 || razorData[7] > 250){
    encoded |= 0x0100 << 9;
    flag = 1;
  }

  if(flag){
    return encoded;
  }
  
  if(razorData[6] < 0){
    encoded |= 0x0100;
  }
  if(razorData[7] < 0){
    encoded |= 0x0100 << 9;
  }
  
  encoded |= razorData[6] << 9;
  encoded |= razorData[7] << 18;
  
  return encoded;
}
uint32_t accelerometer(int razorData[12]){
  uint32_t encoded = 0;
  uint8_t flag = 0;
  if(razorData[9] < -250 || razorData[9] > 250){
    encoded |= 0x0100;
    flag = 1;
  }
  if(razorData[10] < -250 || razorData[10] > 250){
    encoded |= 0x0100 << 9;
    flag = 1;
  }

  if(flag){
    return encoded;
  }
  
  if(razorData[9] < 0){
    encoded |= 0x0100;
  }
  if(razorData[10] < 0){
    encoded |= 0x0100 << 9;
  }
  
  encoded |= razorData[9] << 9;
  encoded |= razorData[10] << 18;
  
  return encoded;
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

void translate(uint16_t receive, int razorData[12], uint8_t data[4]){
  uint16_t request;
  uint32_t package = 0;
 
  request = receive & 0x000F;
  package |= request;

  switch(request){
  case 1:
    package |= infrared(16,8,7) << 4;
    break;
  case 2:
    package |= ultrasonic(630) << 4;
    package |= ultrasonic(630) << 12;
    package |= ultrasonic(630) << 20;
    break;
  case 3:
    package |= magnetometer(razorData);
    break;
  case 4:
    package |= gyroscope(razorData);
    break;
  case 5:
    package |= accelerometer(razorData);
    break;
  }
  package |= TERM << 30;

  data[0]= (package >> 24);
  data[1]= (package >> 16) & 0xFF;
  data[2]= (package >> 8) & 0xFF;
  data[3]= package & 0xFF;
  return data;
}
