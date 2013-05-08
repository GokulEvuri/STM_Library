#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"


#include "USB.h"

#include "usbdescriptor.h"

//#define IN_MULT 1

uint8_t transferBuf[IN_PACKETSIZE*IN_MULT];

/*
 * Application entry point.
 */
int main(void) {

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
   * Activates the USB driver and then the USB bus pull-up on D+.
   */
    myUSBinit();



  /* BaseSequentialStream *chp =   (BaseSequentialStream *)&SDU1; */
  uint16_t i;
  for(i=0;i<sizeof transferBuf;i++)
    transferBuf[i] = i;
 
 

  /*
   * Main loop, does nothing except spawn a shell when the old one was terminated
   */
  while (TRUE) {
    send_data(transferBuf);
    
    chThdSleepMilliseconds(1000);
  }
  
  
}
