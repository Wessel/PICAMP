#ifndef PICAMP_POT_H
#define PICAMP_POT_H

// #[declarations:xc8]
#include <xc.h>
#ifndef _XTAL_FREQ
  #define _XTAL_FREQ 500000
#endif

// #[declarations:adc]
typedef enum {
  // PIC16F887
  ADC_AN00 = 0b0000,
  ADC_AN01 = 0b0001,
  ADC_AN02 = 0b0010,
  ADC_AN03 = 0b0011,
  ADC_AN04 = 0b0100,
  ADC_AN05 = 0b0101,
  ADC_AN06 = 0b0110,
  ADC_AN07 = 0b0111,
  ADC_AN08 = 0b1000,
  ADC_AN09 = 0b1001,
  ADC_AN10 = 0b1010,
  ADC_AN11 = 0b1011,
  ADC_AN12 = 0b1100,
  ADC_AN13 = 0b1101,
  ADC_CVREF = 0b1110, // CVREF
  ADC_FXREF = 0b1111, // Fixed Ref (0.6V fixed voltage reference)
  // PIC16F1829
  ADC_16F_AN00 = 0b00000,
  ADC_16F_AN01 = 0b00001,
  ADC_16F_AN02 = 0b00010,
  ADC_16F_AN03 = 0b00011,
  ADC_16F_AN04 = 0b00100,
  ADC_16F_AN05 = 0b00101,
  ADC_16F_AN06 = 0b00110,
  ADC_16F_AN07 = 0b00111,
  ADC_16F_AN08 = 0b01000,
  ADC_16F_AN09 = 0b01001,
  ADC_16F_AN10 = 0b01010,
  ADC_16F_AN11 = 0b01011,
  ADC_16F_TEMP = 0b11101,
  ADC_16F_DACO = 0b11110,
  ADC_16F_FVRB = 0b11111,
} ADCChannelSelect;

// #[declarations:functions]
void populate_step_array(int *array, int size, int potLimit);
void init_pot(ADCChannelSelect analogPort, volatile unsigned char *pinAddress);
int read_pot(void);

#endif
