#include <stdlib.h>

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

void print(BaseSequentialStream *chp, int argc, char *argv[]) {
  
  (void)argv;
  
  if (argc >0 ) {
    chSequentialStreamPut(chp,'f');// to send data
    //chprintf(chp, "Usage: measure\r\n"); // To print the things
    return;
  }
  
  //  chprintf(chp, "out\r\n"); // to print things

  chSequentialStreamPut(chp,'a');// to send data
  
}
