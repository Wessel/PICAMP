// PIC hardware mapping, global config
#include "../include/config.h"

// Configuration bit structs and enums
#include "../include/initialisation.h"

// #[initialisation:gpio]
/** @brief Initialize GPIO pins on the PIC
 *
 * This function simplifies the configuration of GPIO pins on the
 * MicroChip PIC Microcontroller.
 * The code has been tested on the following microcontrollers:
 *  > PIC16F887A
 *
 * @param config `PinConfig` struct containing the configuration for the pins
 *
 * @return void
 */
//todo (wessel): Find a fix for `bitfield` addressess
void init_gpio(PinConfig config[], int size) {
  // Calculate the size of `config`
  // uint8_t size = sizeof(config) / sizeof(config[0]);

  // Loop through each pin, set their values
  for (int i = 0; i < size; i++) {
    // Set port direction and analog/digital select registers
    *config[i].tris = config[i].direction;
    *config[i].ansel = config[i].mode;

    if (config[i].direction == OUTPUT) {
      // Initialize output pins to low
      *config[i].lat = OFF;
    }
  }
}

/** @brief Initialize the oscillator on the PIC
 *
 * This function simplifies the configuration of the oscillator on the
 * MicroChip PIC Microcontroller.
 * The code has been tested on the following microcontrollers:
 * > PIC16F887A
 *
 * @param config `OscillatorConfig` struct containing the configuration for the oscillator
 *
 * @return void
*/
void init_osc(OscillatorConfig config) {
  // System Clock Select (SCS)
  OSCCONbits.SCS = config.SCS;
  // Internal Resistor-Capacitor Frequency select (IRCF)
  OSCCONbits.IRCF = config.IRCF;
  // OSC startup time
  OSCCONbits.OSTS = config.OSTS;

  // Halt boot until clock is stable
  if (OSCCONbits.IRCF == KHZ032) {
    while (OSCCONbits.LTS != LTS_STABLE);
  } else {
    while (OSCCONbits.HTS != HTS_STABLE);
  }
}

/** @brief Initialize interrupts on the PIC
 *
 * This function simplifies the configuration of interrupts on the
 * MicroChip PIC Microcontroller.
 * The code has been tested on the following microcontrollers:
 * > PIC16F887A
 *
 * @param config `InterruptConfig` struct containing the configuration for the interrupts
 *
 * @return void
 */
void init_int(InterruptConfig config) {
  // Global interrupt enable
  INTCONbits.GIE = config.GIE;
  // External interrupt enable
  INTCONbits.INTE = config.EINT;
  // Peripheral interrupt enable
  // todo(wessel): Add PIR1, PIR2 registers
  INTCONbits.PEIE = config.PEIE;
  // Timer0 Overflow Interrupt enable
  INTCONbits.T0IE = config.T0IE;

  // PORTB Change Interrupt Enable
  INTCONbits.RBIE = config.RBIE;

  // todo(wessel): make modulair
  IOCBbits.IOCB4 = 1;
  IOCBbits.IOCB5 = 1;

  // Set interrupt edge
  OPTION_REGbits.INTEDG = config.INTEDG;

  // Clear interrupt flag
  if (INTCONbits.INTE == EINT_ENABLED) {
    INTCONbits.INTF = INT_AWAITING;
  }
  if (INTCONbits.RBIE == RBINT_ENABLED) {
    INTCONbits.RBIF = INT_AWAITING;
  }
  if (INTCONbits.T0IE == T0INT_ENABLED) {
    INTCONbits.T0IF = INT_AWAITING;
  }
}

/** @brief Initialize the ADC on the PIC
 *
 * This function simplifies the configuration of the analog to
 * digital function on the MicroChip PIC Microcontroller.
 * The code has been tested on the following microcontrollers:
 * > PIC16F887A
 *
 * @param config `ADCConfig` struct containing the configuration for the ADC
 *
 * @return void
*/
// void init_adc(ADCConfig config) {
//   // ADC enable
//   ADCON0bits.ADON = config.ADON;
//   // ADC cycle enable
//   ADCON0bits.GO_nDONE = config.GO_nDONE;
//   // Analog channel select
//   ADCON0bits.CHS = config.CHS;
//   // ADC clock select
//   ADCON0bits.ADCS = config.ADCS;

//   // ADC result alignment
//   ADCON1bits.ADFM = config.ADFM;
//   // ADC voltage reference configuration
//   ADCON1bits.VCFG1 = config.VCFG1;
//   ADCON1bits.VCFG0 = config.VCFG0;
// }