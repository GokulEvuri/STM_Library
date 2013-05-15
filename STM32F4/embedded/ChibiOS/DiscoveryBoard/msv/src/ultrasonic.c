#include <stdlib.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "msv/include/ultrasonic.h"




#define SRF08_addr 0b1110000 //slave device address (7 bits without R/W bit For SRF08 in  Hex: 0xE0/0xE1/0b1110000)
#define SRF08_reg 0x00    /* register address of SRF08*/
#define Ranging_typ 0x51    // Perform ranging, result in CM
#define SRF08_TX_DEPTH 6
#define SRF08_RX_DEPTH 2


msg_t status = RDY_OK;
systime_t tmo = MS2ST(20);


/* I2C interface  */
static const I2CConfig i2cfg1 = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};


/**
 * Converts data from 2complemented representation to signed integer
 */
int16_t complement2signed(uint8_t msb, uint8_t lsb){
    uint16_t word = 0;
    word = (msb << 8) + lsb;
    if (msb > 0x7F){
        return -1 * ((int16_t)((~word) + 1));
    }
    return (int16_t)word;
}


void setRange(void){
  uint8_t txbuf[SRF08_TX_DEPTH];
  uint8_t rxbuf[SRF08_RX_DEPTH];
  txbuf[0] = 0x02;       
  txbuf[1] = 0xFF;      
  i2cAcquireBus(&I2CD1);
  status = i2cMasterTransmitTimeout(&I2CD1, SRF08_addr, txbuf, 2, rxbuf, 0, tmo);
  i2cReleaseBus(&I2CD1);
}

void sendData(void){
  
  uint8_t txbuf[SRF08_TX_DEPTH];
  uint8_t rxbuf[SRF08_RX_DEPTH];
  txbuf[0] = SRF08_reg;       /* register address */
  txbuf[1] = Ranging_typ;        
  i2cAcquireBus(&I2CD1);
  status = i2cMasterTransmitTimeout(&I2CD1, SRF08_addr, txbuf, 2, rxbuf, 0, tmo);
  i2cReleaseBus(&I2CD1);
}



void receiveData(void){
   uint8_t txbuf[SRF08_TX_DEPTH];
   uint8_t rxbuf[SRF08_RX_DEPTH];
   txbuf[0] = 0x02; //read data from location 2
   i2cAcquireBus(&I2CD1);
   status=i2cMasterTransmitTimeout(&I2CD1, SRF08_addr, txbuf, 1, rxbuf, 2, tmo);
   range = complement2signed(rxbuf[0], rxbuf[1]);
   i2cReleaseBus(&I2CD1);
}

static WORKING_AREA(waThread2, 128);
static msg_t Thread2(void *arg) {
  
  (void)arg;
  chRegSetThreadName("ultrasonic");
  // Reader thread loop.
   while (TRUE) {
  sendData();
  chThdSleepMilliseconds(75);
  receiveData();
  chThdSleepMilliseconds(75);
  }
  chThdSleepMilliseconds(10);
  return (msg_t)0;
}

void cmd_printDataFromUltrasonic(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)chp;
  (void)argc;
  (void)argv;
  chprintf(chp, "Distance data in centimeter");
  chprintf(chp, "\r\n");
  chprintf(chp, "distance=%d\n",range);
  chprintf(chp, "\r\n");
}



void myUltrasonicInit(void) {

  i2cStart(&I2CD1, &i2cfg1);
  palSetPadMode(GPIOB, 6, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);   /* SCL */
  palSetPadMode(GPIOB, 9, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);   /* SDA */
 //setRange();
 chThdCreateStatic(waThread2, sizeof(waThread2),
                    NORMALPRIO + 10, Thread2, NULL);
}


int16_t* getValue() {
	return range;
}
