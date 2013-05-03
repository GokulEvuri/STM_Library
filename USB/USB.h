#ifndef USB_H_INCLUDED
#define USB_H_INCLUDED
extern SerialUSBDriver SDU1;

void myUSBinit(void);
int isUsbActive(void);

void send_data(uint8_t *buf);
uint8_t receive_data(void);

#endif // USB_H_INCLUDED
