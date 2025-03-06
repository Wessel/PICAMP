#include "../global/picamp-potentiometer.h"
#include "../global/registers.h"
// #include <xc.h>

void _pot_delay() {
  unsigned char counter = 1; // Delay Cycle to achieve 500Us delay (165)
  while (--counter != 0)     // Delay Timing is approximately 3 (3*2*2*2=24) usec per loop
    continue;                // Note this routine is for 4MHz (500khz) crystal frequency
}

void init_pot(picId id, ADCChannelSelect channel) {
  switch (id) {
    case PIC16F1829: _pot_init_1829(channel); break;
    case PIC16F887A: _pot_init_887(channel); break;
  }
}

//todo: add tris and ansel bit fields
void _pot_init_887(ADCChannelSelect channel) {

  SetRawReg((unsigned char *)0x01F, 1, 0, 1);       // ADON
  SetRawReg((unsigned char *)0x01F, 4, 2, channel); // CHS
  SetRawReg((unsigned char *)0x01F, 2, 6, 0b00);    // ADCS
  SetRawReg((unsigned char *)0x09F, 2, 4, 0b00);    // VCFG
  SetRawReg((unsigned char *)0x09F, 1, 7, 1);       // ADFM

  ADC = (ADCPins) {
    .GOnDONE = { (unsigned char*) 0x01F, 1, 1 },
    .ADRESH  = { (unsigned char*) 0x01E, 8, 0 },
    .ADRESL  = { (unsigned char*) 0x09E, 8, 0 },
  };

  // Memory space is limited on the pic, so we group the registers with the same value.
  // .VCFG0   = { (unsigned char*) 0x01F, 1, 4 },
  // .VCFG1   = { (unsigned char*) 0x01F, 1, 5 },
  // .ADCS    = { (unsigned char*) 0x01F, 1, 6 },
  // SetReg(&ADC.ADCS, 0);
  // SetReg(&ADC.VCFG0, 0);
  // SetReg(&ADC.VCFG1, 0);
//    .ADON     = { (unsigned char*) 0x01F, 1, 0 },
//    .CHS      = { (unsigned char*) 0x01F, 4, 2 },
//    .ADCS     = { (unsigned char*) 0x01F, 2, 6 },
//    .VCFG     = { (unsigned char*) 0x09F, 2, 4 },
//    .ADFM     = { (unsigned char*) 0x09F, 1, 7 },

  // SetRawReg((unsigned char*) 0x01F, 1, 0, 1);

//  SetReg(&ADC.ADON, 1);
  SetReg(&ADC.GOnDONE, 1);
//  SetReg(&ADC.CHS, channel);
//  SetReg(&ADC.VCFGADCS, 0b000);
//  SetReg(&ADC.ADFM, 1);
}

void _pot_init_1829(ADCChannelSelect channel) {

  SetRawReg((unsigned char *)0x09D, 1, 0, 1);       // ADON
  SetRawReg((unsigned char *)0x09D, 5, 2, channel); // CHS
  SetRawReg((unsigned char *)0x09E, 5, 0, 0b00000); // VCFGADCS
  SetRawReg((unsigned char *)0x09E, 1, 7, 1);       // ADFM

  ADC = (ADCPins) {
    .GOnDONE = { (unsigned char*) 0x09D, 1, 1 },
    .ADRESH  = { (unsigned char*) 0x09C, 8, 0 },
    .ADRESL  = { (unsigned char*) 0x09B, 8, 0 },
  };

  // Just like the 887, memory space is limited. Structs take up a lot of space,
  // so we group the registers with the same value.
  //    .ADON     = { (unsigned char*) 0x09D, 1, 0 },
  //    .CHS      = { (unsigned char*) 0x09D, 5, 2 },
  //    .VCFGADCS = { (unsigned char*) 0x09E, 5, 0 },
  //    .ADFM     = { (unsigned char*) 0x09E, 1, 7 },
  // .VCFG0   = { (unsigned char*) 0x09E, 2, 0 },
  // .VCFG1   = { (unsigned char*) 0x09E, 1, 2 },
  // .ADCS    = { (unsigned char*) 0x09E, 3, 4 },
  // SetReg(&ADC.VCFG0, 0);
  // SetReg(&ADC.VCFG1, 0);
  // SetReg(&ADC.ADCS, 0);
    // structs are only needed for re-used values outside of this function

//  SetReg(&ADC.ADON, 1);
  SetReg(&ADC.GOnDONE, 1);
//  SetReg(&ADC.CHS, channel);
//  SetReg(&ADC.VCFGADCS, 0b00000);
//  SetReg(&ADC.ADFM, 1);
}

void populate_step_array(int *array, int size, int potLimit) {
  array[0] = potLimit / (size + 1);
  for (int i = 0; i < size; i++) {
    array[i] = array[0] * (i + 1);
  }
}

int read_potentiometer() {
  // Wait for regulating capacitor to charge
  _pot_delay();

  // Bit 2 of `ADCON0` is set to `1`, meaning `GO` will wait until
  // ADC got a signal, converted analog to digital, etc.
  SetReg(&ADC.GOnDONE, 1);
  while (GetReg(&ADC.GOnDONE)) continue;

  // Read the data from the potmeter pin (`E2`) registers, and combine
  // them into a single integer
  return (GetReg(&ADC.ADRESH) << 8 | GetReg(&ADC.ADRESL));
}

//todo: fix!
int adc_to_step(int *stepArray, int size, int deadzone) {
    return 0;
}