#ifndef ROTARY_H
#define ROTARY_H

#include "registers.h"

// intcon = 0x0B
// OPTION_REG = 0x81
// IOCB = 0x96
// pin 17 = int pin?

typedef enum { ROTATE_LEFT = 1, ROTATE_RIGHT = 2 } RotaryEncoderStates;
typedef enum {
  IOCRB0 = 0,
  IOCRB1 = 1,
  IOCRB2 = 2,
  IOCRB3 = 3,
  IOCRB4 = 4,
  IOCRB5 = 5,
  IOCRB6 = 6,
  IOCRB7 = 7
} IOCPin;

typedef struct _Encoder {
  int LastState;
  int Steps;
  int Input;
  int min;
  int max;
  int stepReq;
  Reg pinA;
  Reg pinB;
  Reg encoderRail;
  picId host;
} Encoder;

Encoder encoder = {0, 0, 0};

void _rot_init_887(IOCPin pinA, IOCPin pinB);
void _rot_init_1829();
void init_rot(picId id);
//void read_rotary(char pA, char pB, char *counter, char max, char min);
int read_rotary(void);

#endif