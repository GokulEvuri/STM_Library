#ifndef RAZOR_H_INCLUDED
#define RAZOR_H_INCLUDED

static int razorData[12];
void cmd_printDataFromRazor(BaseSequentialStream *chp, int argc, char *argv[]);
void myRazorInit(void);

#endif // RAZOR_H_INCLUDED
