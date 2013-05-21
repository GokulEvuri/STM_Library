#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include "PWM/PWM.h"
#include "ADC/ADC.h"
#include "USB/USB.h"
#include "Misc/Misc.h"
#include "SPI/SPI.h"
#include "msv/include/RAZOR.h"
#include "msv/include/protocol_byte.h"
#include "msv/include/motor.h"
#include "msv/include/ultrasonic.h"
#include "msv/include/IMU.h"

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
  uint8_t receivedBuff[2]; // Received request/information from PandaBoard
  uint8_t sentData[4];     // Returned Information (reply) to the PandaBoard
  float imuData[7];        // IMU calculated data based on Razor Boad
  int* razorInfo;          // Razor Board Data
  
  /*imuData[0] = 1;
    imuData[1] = 2;
    imuData[2] = 3;
    imuData[3] = 1;
    imuData[4] = 1000;
    imuData[5] = 4;
    imuData[6] = 5;*/

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
  /*
  motorInit(); 
  mypwmInit();
  myADCinit();*/
  
  // Initializing Discovery Board's Accelerometer
  // mySPIinit();

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
    uint16_t receivedByte = (uint16_t)atol(receivedBuff);
 
    razorInfo = getRazorValues();
    getImuValues(imuData);
    getAccel(accelData);
    
    //setMotorData((receivedByte >> 4) & 0x3F,(receivedByte >> 4) & 0x3F);

    translate(receivedByte,razorInfo,imuData,accelData,sentData);
    sdWrite(&SDU1, sentData, 4);
    
   
    //Controlled Delay
    chThdSleepMilliseconds(10);
  }
}


// Handling requests 8 and 9, aren't used curently

/* if((receivedByte & 0x000F) == 8){
        translate(3,razorData,accelData,sentData);
        sdWrite(&SDU1, sentData, 4);
	translate(4,razorData,accelData,sentData);
        sdWrite(&SDU1, sentData, 4);
        translate(5,razorData,accelData,sentData);
        sdWrite(&SDU1, sentData, 4);
        translate(6,razorData,accelData,sentData);
        sdWrite(&SDU1, sentData, 4);
    }
    if((receivedByte & 0x000F) == 8){
        translate(3,razorData,accelData,sentData);
        sdWrite(&SDU1, sentData, 4);

	translate(4,razorData,accelData,sentData);
        sdWrite(&SDU1, sentData, 4);
        translate(5,razorData,accelData,sentData);
        sdWrite(&SDU1, sentData, 4);
        translate(6,razorData,accelData,sentData);

        sdWrite(&SDU1, sentData, 4);
        translate(7,razorData,accelData,sentData);
        sdWrite(&SDU1, sentData, 4);
    }*/
