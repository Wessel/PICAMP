#ifndef PICAMP_ROTARY_H
#define PICAMP_ROTARY_H

#include <xc.h>
#ifndef _XTAL_FREQ
  #define _XTAL_FREQ 500000
#endif

// #[declarations:rotary]
typedef enum { ROTATE_LEFT = 1, ROTATE_RIGHT = 2 } RotaryEncoderStates;

// #[declarations:functions]
void parse_rotary(char A, char B, char* counter, char max, char min);
void init_rotary(volatile unsigned char *trisA, volatile unsigned char *trisB);

#endif
