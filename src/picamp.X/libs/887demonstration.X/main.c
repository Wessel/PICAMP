#include "./config.h"

#include <xc.h>
#include "../global/picamp-potentiometer.h"
#include "../global/picamp-rotary.h"

// Function declarations
void pic_init(void);
void init_gpio(void);
void init_osc(void);
void pic_loop(void);

int count = 0;

void main(void) {
  // Do all initialisation here
  pic_init();
  // Start indefinite program loop
  pic_loop();
}

void pic_loop(void) {
  // Initialize potentiometer step array
  int stepArray[LED_AMOUNT] = { 0 };
  populate_step_array(stepArray, LED_AMOUNT, POT_LIMIT);

  while (1) {
    // Do not display the LEDs if input selector is > 0
    if (count != 0) continue;
    // Parse result from potentiometer
    int result = read_potentiometer();
    // Read the data from the potmeter pin (`E2`) registers, and combine
    // them into a single integer
    // Loop through all `stepArray`, check if the output of the potmeter
    // (on `ADRES`) falls inbetween the steps - `DEADZONE`, if so execute
    // An OR/AND bitshift operation in order to turn on the corresponding
    // LED on `PORTA`.
    for (int i = 0; i < LED_AMOUNT; i++) {
      if (result > (stepArray[i] - DEADZONE)) {
        PORTA = (unsigned char)(PORTA & ~(1 << i));
      } else if (result <= (stepArray[i] - DEADZONE)) {
        PORTA = (unsigned char)(PORTA | (1 << i));
      }
    }
    // int step = adc_to_step(*stepArray, LED_AMOUNT, DEADZONE);
    // PORTA = (unsigned char)(PORTA | (1 << step));
  }
}

void pic_init(void) {
  init_osc();
  init_pot(PIC16F887A, P887A_AN07);
  init_rot(PIC16F887A);
  init_gpio();
}

// #[initialisation:gpio]
void init_gpio(void) {
  // LED => output, digital (transistors)
  TRISA = 0x00;
  PORTA = 0xFF;
  ANSEL = 0;
}

// #[initialisation:osc]
void init_osc(void) {
  // System Clock Select (SCS)
  OSCCONbits.SCS = 0b1; // Internal Clock selected
  // Internal Resistor-Capacitor Frequency select (IRCF)
  OSCCONbits.IRCF = 0b011; // 500 kHz clock speed
  OSCCONbits.OSTS = 0b1; // OSC startup time

  // Halt boot until clock is stable
  while (OSCCONbits.HTS != 0b1);
}

// #[routine:interrupt_service]
void __interrupt() isr(void) {
 if (INTCONbits.RBIF == 1) {
   // Parse rotary changes into `count`, shift it into `PORTA`
    count = read_rotary();
    PORTA = (unsigned char)(~(1 << count));

    // Clear the Port B change interrupt flag
    INTCONbits.RBIF = 0;
  }
}
