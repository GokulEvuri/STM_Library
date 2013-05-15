#ifndef ULTRASONIC_H_INCLUDED
#define ULTRASONIC_H_INCLUDED

static int16_t range;
void cmd_printDataFromUltrasonic(BaseSequentialStream *chp, int argc, char *argv[]);
void myUltrasonicInit(void);
int16_t* getValue();

#endif // ULTRASONIC_H_INCLUDED
