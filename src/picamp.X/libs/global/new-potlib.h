#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "registers.h"
// #[declarations:adc]
typedef enum _ADCChannelSelect {
  // PIC16F887
  P887A_AN00 = 0b0000,
  P887A_AN01 = 0b0001,
  P887A_AN02 = 0b0010,
  P887A_AN03 = 0b0011,
  P887A_AN04 = 0b0100,
  P887A_AN05 = 0b0101,
  P887A_AN06 = 0b0110,
  P887A_AN07 = 0b0111,
  P887A_AN08 = 0b1000,
  P887A_AN09 = 0b1001,
  P887A_AN10 = 0b1010,
  P887A_AN11 = 0b1011,
  P887A_AN12 = 0b1100,
  P887A_AN13 = 0b1101,
  P887A_CVREF = 0b1110, // CVREF
  P887A_FXREF = 0b1111, // Fixed Ref (0.6V fixed voltage reference)
  // PIC16F1829
  P1829_AN00 = 0b00000,
  P1829_AN01 = 0b00001,
  P1829_AN02 = 0b00010,
  P1829_AN03 = 0b00011,
  P1829_AN04 = 0b00100,
  P1829_AN05 = 0b00101,
  P1829_AN06 = 0b00110,
  P1829_AN07 = 0b00111,
  P1829_AN08 = 0b01000,
  P1829_AN09 = 0b01001,
  P1829_AN10 = 0b01010,
  P1829_AN11 = 0b01011,
  P1829_TEMP = 0b11101,
  P1829_DACO = 0b11110,
  P1829_FVRB = 0b11111,
} ADCChannelSelect;

typedef struct _ADCPins {
  Reg GOnDONE;
  Reg ADRESH;
  Reg ADRESL;
} ADCPins;

ADCPins ADC;

void init_pot(picId id, ADCChannelSelect channel);
void _pot_init_887(ADCChannelSelect channel);
void _pot_init_1829(ADCChannelSelect channel);
int read_potentiometer(void);
void populate_step_array(int *array, int size, int potLimit);
int adc_to_step(int *stepArray, int size, int deadzone);

#endif