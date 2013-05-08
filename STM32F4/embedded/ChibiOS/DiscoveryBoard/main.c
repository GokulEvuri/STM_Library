#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"


#include "USB/USB.h"


#include "test_stmUSB.h"


/*
 * assert Shell Commands to functions
 */

static const ShellCommand commands[] = {
  {"print", print},
  {NULL, NULL}
};


/*
 * Shell configuration
 */

#define SHELL_WA_SIZE   THD_WA_SIZE(2048)

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
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
   * Activates the USB driver and then the USB bus pull-up on D+.
   */
  myUSBinit();


  /*
   * Main loop, does nothing except spawn a shell when the old one was terminated
   */
  while (TRUE) {
    if (!shelltp && isUsbActive())
      {
        shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
      }
    else if (chThdTerminated(shelltp)) {
      chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
      shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }

    chThdSleepMilliseconds(1000);
  }

}
