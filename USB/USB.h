#ifndef USB_H_INCLUDED
#define USB_H_INCLUDED

/* #include "usbdescriptor.h" */

/* uint8_t receiveBuf[OUT_PACKETSIZE]; */
/* #define IN_MULT 4 */
/* uint8_t transferBuf[IN_PACKETSIZE*IN_MULT]; */

/* USBDriver *  	usbp = &USBD1; */
/* uint8_t initUSB=0; */
/* uint8_t usbStatus = 0; */

void myUSBinit(void);
int isUsbActive(void);

void send_data(void);
uint8_t receive_data(void);


#endif // USB_H_INCLUDED 
