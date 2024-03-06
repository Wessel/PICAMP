#include "picamp-potentiometer.h"

#define PIC887A
// #[config:887a]
#ifdef PIC887A
  int __get_pod_data(void) {
    // Combines the high and low registers to give one 16 bit value
    return (ADRESH << 8 | ADRESL);
  }

  int __init_adc(ADCChannelSelect analogPin) {
    // ADC enable
    ADCON0bits.ADON = 1; // ADC_ON
    // ADC cycle enable
    ADCON0bits.GO_nDONE = 1; // ADC_IN_PROGRESS
    // Analog channel select
    ADCON0bits.CHS = analogPin; // ADC_AN07
    // ADC clock select
    ADCON0bits.ADCS = 0; // ADC_FOSC02

    // ADC result alignment
    ADCON1bits.ADFM = 1; // ADC_RIGHT
    // ADC voltage reference configuration
    ADCON1bits.VCFG1 = 0; // VREF_PLUSPIN
    ADCON1bits.VCFG0 = 0; // VREF_MINPIN

    // Set the analog pin to analog mode
    switch (analogPin) {
      case ADC_AN00: ANSELbits.ANS0 = 1; break;
      case ADC_AN01: ANSELbits.ANS1 = 1; break;
      case ADC_AN02: ANSELbits.ANS2 = 1; break;
      case ADC_AN03: ANSELbits.ANS3 = 1; break;
      case ADC_AN04: ANSELbits.ANS5 = 1; break;
      case ADC_AN05: ANSELbits.ANS5 = 1; break;
      case ADC_AN06: ANSELbits.ANS6 = 1; break;
      case ADC_AN07: ANSELbits.ANS7 = 1; break;
      case ADC_AN08: ANSELHbits.ANS8 = 1; break;
      case ADC_AN09: ANSELHbits.ANS9 = 1; break;
      case ADC_AN10: ANSELHbits.ANS10 = 1; break;
      case ADC_AN11: ANSELHbits.ANS11 = 1; break;
      case ADC_AN12: ANSELHbits.ANS12 = 1; break;
      case ADC_AN13: ANSELHbits.ANS13 = 1; break;
      default: break;
    }

    return 1;
  }
#elif PIC1829
int __get_pod_data(void) {
  return ADRES;
}

void __init_adc(ADCChannelSelect analogPin) {
  // ADCON0 = 0b00001111;
  ADCON0bits.ADON = 1; // ADC_ON
  ADCON0bits.GO_nDONE = 1; // ADC_IN_PROGRESS
  ADCON0bits.CHS = analogPin; // ADC_CGANNEL_SELECT

  ADCON1bits.ADFM = 1;

  // A line = input line
  // todo: fix! tris and ansel
  // A4 = potmeter pin, set as input
  // TRISAbits.TRISA4 = 1;

  // // A4 = potmeter = analog, set to 1
  // ANSELAbits.ANSA4 = 1;
  switch (analogPin) {
    case ADC_16F_AN00: ANSELAbits.ANSA0 = 1; break;
    case ADC_16F_AN01: ANSELAbits.ANSA1 = 1; break;
    case ADC_16F_AN02: ANSELAbits.ANSA2 = 1; break;
    case ADC_16F_AN04: ANSELCbits.ANSC0 = 1; break;
    case ADC_16F_AN05: ANSELCbits.ANSC1 = 1; break;
    case ADC_16F_AN06: ANSELCbits.ANSC2 = 1; break;
    case ADC_16F_AN10: ANSELBbits.ANSB4 = 1; break;
    case ADC_16F_AN11: ANSELBbits.ANSB5 = 1; break;

  }
}
#endif

/** @brief Initialize the potentiometer
 *
 * This function initializes the potentiometer by setting the config bits
 * according to the current PIC microcontroller used. It also sets the pin
 * connected to the potentiometer to output mode.
 *
 * @param analogPort The analog port to use
 * @param pinAddress The address of the pin to use
 *
 * @return void
 */
void init_pot(ADCChannelSelect analogPort, volatile unsigned char* pinAddress) {
 // Set up ADC based on the PIC used
  __init_adc(analogPort);
  // Set the pin connected to the potentiometer to output mode
  *pinAddress = 1;
}

/** @brief Populate an array with step values
 *
 * This function populates an array with step values, based on the size of the
 * array and the potentiometer limit. The first value in the array is the
 * maximum value, the last value is the minimum value.
 *
 * @param array The array to populate
 * @param size The size of the array
 * @param potLimit The maximum value of the potentiometer
 *
 * @return void
 */
void populate_step_array(int *array, int size, int potLimit) {
  // All subsequent steps are based off of the first array entry
  array[0] = potLimit / (size + 1);
  // Populate the array with equally divided step values
  for (int i = 0; i < size; i++) {
    array[i] = array[0] * (i + 1);
  }
}


/** @brief Read the potentiometer
 *
 * This function reads the potentiometer by setting the `GO` bit to `1` and
 * waiting for the ADC to finish. It then returns the value of the ADC.
 *
 * @return int The value of the ADC
 */
int read_pot(void) {
  // Wait for regulating capacitor to charge
  __delay_us(5);

  // Bit 2 of `ADCON0` is set to `1`, meaning `GO` will wait until
  // ADC got a signal, converted analog to digital, etc.
  GO = 1;
  while (GO) continue;

  // Read the data from the potmeter pin (`E2`) registers, and combine
  // them into a single integer
  return __get_pod_data();
}
