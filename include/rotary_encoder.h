#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

typedef enum { ROTATE_LEFT = 1, ROTATE_RIGHT = 2 } RotaryEncoderStates;

// volatile unsigned char A, volatile unsigned char B
void parse_rotary(char pinA, char pinB, char *counter, char max, char min);

#endif
