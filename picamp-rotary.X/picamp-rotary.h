#ifndef PICAMP_ROTARY_H
#define PICAMP_ROTARY_H

#include <xc.h>
#ifndef _XTAL_FREQ
  #define _XTAL_FREQ 500000
#endif

// #[declarations:rotary]
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

// #[declarations:functions]
void parse_rotary(char REA, char REB, char* counter, char max, char min);
void init_rotary(IOCPin RBA, IOCPin RBB, volatile unsigned char* trisA, volatile unsigned char* trisB);

#endif
