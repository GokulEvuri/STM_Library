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

void testProtocol(BaseSequentialStream *chp, int argc, char *argv[]);

/*
* assert Shell Commands to functions
*/
static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"measure", cmd_measure},
  {"m", cmd_measure},
  {"measureAnalog", cmd_measureA},
  {"ma", cmd_measureA},
  {"vref", cmd_Vref},
  {"v", cmd_Vref},
  {"temperature", cmd_Temperature},
  {"te", cmd_Temperature},
  {"measureDirect", cmd_measureDirect},
  {"md", cmd_measureDirect},
  {"measureContinuous", cmd_measureCont},
  {"mc", cmd_measureCont},
  {"readContinuousData", cmd_measureRead},
  {"rd", cmd_measureRead},
  {"stopContinuous", cmd_measureStop},
  {"sc", cmd_measureStop},
  {"printAccel", cmd_printAccel},
  {"pa", cmd_printAccel},
  {"rz", cmd_printDataFromRazor},
  {"test", testProtocol},
  {NULL, NULL}
};

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
  
  /*
   * Shell thread
   */
  Thread *shelltp = NULL;

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
   * Activate custom stuff
   */
  /*mypwmInit();
  myADCinit();
  mySPIinit();
  myRazorInit();*/

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   */
  myUSBinit();

  sdStart(&SDU1,&portConfig2);
  uint8_t receivedBuff[2];
  uint8_t data[4];
  /*
   * Main loop, does nothing except spawn a shell when the old one was terminated
   */

  while (TRUE) {
    sdRead(&SDU1, receivedBuff, 4);
    uint16_t receivedByte= (uint16_t)atol(receivedBuff);
    translate(receivedByte,razorData,data);
    sdWrite(&SDU1, data, 4);

    //sleep for a while
    chThdSleepMilliseconds(10);
  }
}

void testProtocol(BaseSequentialStream *chp, int argc, char *argv[]){
    int *razorData2, i; 
    uint8_t data[4];
    razorData2=getValues();
    chprintf(chp,"Rz Data: %d\r\n", razorData2[3], razorData2[4]);
    uint16_t receive = 2;
    translate(receive,razorData2,data);
    chprintf(chp,"Byte Data: ");
    for(i = 0; i<4; i++)
    chprintf(chp,"%x", data[i]);
    chprintf(chp,"\r\n");
}
