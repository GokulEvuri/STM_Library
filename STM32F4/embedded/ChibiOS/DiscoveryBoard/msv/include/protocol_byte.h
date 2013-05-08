#ifndef RAZOR_PROTOCOL_H
#define RAZOR_PROTOCOL_H

void razorToBits(int razorData[12], unsigned char* byte);//has to be changed!!!
uint8_t ultrasonic(int value);
uint32_t infrared(int ir1, int ir2, int ir3);
uint32_t translate(uint16_t receive);


#endif
