#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"


#include "USB.h"



/* static const ShellConfig shell_cfg1 = { */
/*   (BaseSequentialStream *)&SDU1, */
/*   commands */
/* }; */

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



  BaseSequentialStream *chp =   (BaseSequentialStream *)&SDU1;

  static uint8_t j[] = "0123456789abcdef";

  /*
   * Main loop, does nothing except spawn a shell when the old one was terminated
   */
  while (TRUE) {
    if (isUsbActive())
      {
	//       uint8_t i = receive_data();
	//chprintf(chp,"Got %d \n", i);
	send_data(j);
	//chprintf(chp,"sent data \n");
      }
    chThdSleepMilliseconds(1000);
  }
}
