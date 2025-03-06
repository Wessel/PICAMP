#include "config.h"

#include <xc.h>
#include "../global/picamp-potentiometer.h"
#include "../global/picamp-rotary.h"

// Function declarations
void pic_init(void);
void init_osc(void);
void init_gpio(void);
void pic_loop(void);

int count = 0;

void main(void) {
  // Do all initialisation here
  pic_init();
  // Start the main program loop
  pic_loop();
}

void pic_loop(void) {
  // Initalize and populate the `PotSteps` array, divides `POT_LIMIT`
  // into increments based on `LED_AMOUNT`. `PotSteps[0]` is different
  // than the rest, therefore it is defined first.
  int PotSteps[LED_AMOUNT];
  populate_step_array(PotSteps, LED_AMOUNT, POT_LIMIT);

  while (1) {
    // Do not display the LEDs if input selector is > 0
    if (count != 0) continue;

    int result = read_potentiometer();
    // Loop through all `PotSteps`, check if the output of the potmeter
    // (on `ADRES`) falls inbetween the steps - `DEADZONE`, if so execute
    // An OR/AND bitshift operation in order to turn on the corresponding
    // LED on `LATC`.
    for (int i = 0; i < LED_AMOUNT; i++) {
      if (result > (PotSteps[i] - DEADZONE)) {
        LATC = (unsigned char)(LATC & ~(1 << i));
      } else if (result <= (PotSteps[i] - DEADZONE)) {
        LATC = (unsigned char)(LATC | (1 << i));
      }
    }
  }
}

void pic_init(void) {
  init_osc();
  init_gpio();
  init_pot(PIC16F1829, P1829_AN03);
  init_rot(PIC16F1829);
}

void init_gpio(void) {
  // LED => output
  TRISC = 0;
  LATC = 0;
}

void init_osc(void) {
  // System Clock Select (SCS)
  OSCCONbits.SCS = 0b00;     // Clk determined by FOSC in Conf-1
  // Internal Resistor-Capacitor Frequency select (IRCF)
  OSCCONbits.IRCF = 0b0111;  // 500 kHz clock speed
  // Software Phase-Locked Loop ENable (SPLLEN)
  OSCCONbits.SPLLEN = 0b0;   // 4 x Phase-Locked Loop disabled
}

void __interrupt() isr() {
  if (INTCONbits.IOCIF) {
    count = read_rotary();
    PORTC = (unsigned char)(~(1 << count));
    INTCONbits.IOCIF = 0;
  }
}