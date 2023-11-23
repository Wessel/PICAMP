#include "../include/config.h"

void populate_step_array(int *array, int size, int potLimit) {
  array[0] = potLimit / (size + 1);
  for (int i = 0; i < size; i++){
    array[i] = array[0] * (i + 1);
  }
}

int read_potentiometer(void) {
  // Wait for regulating capacitor to charge
  __delay_us(5);

  // Bit 2 of `ADCON0` is set to `1`, meaning `GO` will wait until
  // ADC got a signal, converted analog to digital, etc.
  GO = 1;
  while (GO) continue;

  // Read the data from the potmeter pin (`E2`) registers, and combine
  // them into a single integer
  int result = (ADRESH * 256) + ADRESL;

  return result;
}
