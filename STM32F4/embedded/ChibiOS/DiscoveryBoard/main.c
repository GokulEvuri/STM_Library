#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include "PWM/PWM.h"
#include "USB/USB.h"
#include "Misc/Misc.h"
#include "SPI/SPI.h"
#include "msv/include/RAZOR.h"
#include "msv/include/protocol_byte.h"
#include "msv/include/motor.h"
#include "msv/include/ultrasonic.h"
#include "msv/include/IMU.h"
#include "msv/include/ir.h"


const SerialConfig portConfig2 = {
    115000,
    0,
    USART_CR2_STOP1_BITS | USART_CR2_LINEN,
    USART_CR3_CTSE
};

/*
* Application entry point.
*/
int main(void) {
  int8_t accelData[2];     // Discovery Board's Accelerometer
  uint8_t receivedBuff[4];// Received request/information from PandaBoard
  uint8_t sentData[4] = {0,0,0,0};     // Returned Information (reply) to the PandaBoard
  float imuData[7];        // IMU calculated data based on Razor Boad
  int* razorInfo;          // Razor Board Data
  int steering = 0;
  int ir_data[3]; 

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit(); 
  
  mypwmInit();
   
 // Initializing Motor
 // motorInit();

  // Initializing IR Thread
 // ADCinit();

  // Initializing US Thread
//  myUltrasonicInit();
  
  // Initializing Discovery Board's Accelerometer
  //mySPIinit();

  // Initializing Razor Board
  myRazorInit();

  // Activates the USB driver and then the USB bus pull-up on D+.
  myUSBinit();

  // Initializing IMU Calculations.
  initIMU();

  //Starting the usb configuration
  sdStart(&SDU1,&portConfig2);
 
  /*
   * Main loop, it takes care of reciving the requests from Panda Board using USB protocol,
   * and reply with the requested data.
   */
  while (TRUE) {
    sdRead(&SDU1, receivedBuff, 4);
    uint32_t receivedByte = (uint32_t)atol(receivedBuff);

    razorInfo = getRazorValues();
    getImuValues(imuData);
    //   getAccel(accelData);
    getIR(ir_data);

    if(receivedByte != 0){
       steering = ((receivedByte)>>2)&0x3F;
       setMotorData(steering-28,1765);

       translate(receivedByte,ir_data,razorInfo,imuData,accelData,sentData);
       sdWrite(&SDU1, sentData, 4);
    }

    //Controlled Delay
    chThdSleepMilliseconds(10);
  }
}
