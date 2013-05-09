#ifndef RAZOR_PROTOCOL_H
#define RAZOR_PROTOCOL_H

#include <stdint.h>

uint32_t yaw(int razorData[12]);
uint32_t magnetometer(int razorData[12]);
uint32_t gyroscope(int razorData[12]);
uint32_t accelerometer(int razorData[12]);
uint8_t ultrasonic(int value);
uint32_t infrared(int ir1, int ir2, int ir3);
uint32_t translate(uint16_t receive, int razorData[12]);


#endif
