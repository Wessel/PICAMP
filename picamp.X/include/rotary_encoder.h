#ifndef PICAMP_ROTARY_H
#define PICAMP_ROTARY_H

#include <xc.h>
#ifndef _XTAL_FREQ
  #define _XTAL_FREQ 500000
#endif

// #[declarations:rotary]
typedef enum { ROTATE_LEFT = 1, ROTATE_RIGHT = 2 } RotaryEncoderStates;
typedef enum {
  IOCB0 = 0,
  IOCB1 = 1,
  IOCB2 = 2,
  IOCB3 = 3,
  IOCB4 = 4,
  IOCB5 = 5,
  IOCB6 = 6,
  IOCB7 = 7
} IOCPin;

// #[declarations:functions]
void parse_rotary(char REA, char REB, char* counter, char max, char min);
void init_rotary(volatile unsigned char* trisA, volatile unsigned char* trisB, IOCPin RBA, IOCPin RBB);

#endif
