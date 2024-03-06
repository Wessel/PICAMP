// Rotary encoder structs and enums
#include "./picamp-rotary.h"

#define PIC16887A
#ifdef PIC16887A
void __init_int(void) {
  // Global interrupt enable
  INTCONbits.GIE = 1;
  // External interrupt enable
  INTCONbits.INTE = 1;
  // Peripheral interrupt enable
  // todo(wessel): Add PIR1, PIR2 registers
  INTCONbits.PEIE = 1;

  // PORTB Change Interrupt Enable
  INTCONbits.RBIE = 1;

  // todo(wessel): make modulair
  IOCBbits.IOCB4 = 1;
  IOCBbits.IOCB5 = 1;

  // Set interrupt edge
  OPTION_REGbits.INTEDG = 0;

  // Clear interrupt flag
  INTCONbits.INTF = 0;
  INTCONbits.RBIF = 0;
}

void __init_pin(volatile unsigned char* trisA, volatile unsigned char* trisB) {
  // Set `trisA` and `trisB` as inputs
  *trisA = 1;
  *trisB = 1;

  // Set all channels to digital
  ANSEL = 0;
  ANSELH = 0;
}
#elif PIC1829
void __init_int(void) {}
void __init_pin(volatile unsigned char* trisA, volatile unsigned char* trisB) {
  // Set `trisA` and `trisB` as inputs
  *trisA = 1;
  *trisB = 1;

  // Set all channels to digital
  ANSELA = 0;
  ANSELB = 0;
  ANSELC = 0;
}
#endif

/** @brief Initialize the rotary encoder
 *
 * This function initializes the rotary encoder by setting the `trisA` and `trisB`
 * as inputs.
 *
 * @param trisA The address of the TRISA register
 * @param trisB The address of the TRISB register
 *
 * @return void
*/
void init_rotary(volatile unsigned char* trisA, volatile unsigned char* trisB) {
  // Initialize the interrupt
  __init_int();

  // Initialize the pins
  __init_pin(trisA, trisB);
};

/** @brief Parse the rotary encoder
 *
 * This function parses the rotary encoder by reading the `REA` and `REB` pins
 * and updating the `counter` accordingly. It also checks if the `counter` is
 * at the maximum or minimum value and loops around if so.
 *
 * @param REA The REA pin
 * @param REB The REB pin
 * @param counter The counter to update
 * @param max The maximum value of the counter
 * @param min The minimum value of the counter
 *
 * @return void
*/
void parse_rotary(char REA, char REB, char* counter, char max, char min) {
  // static values retain their state, even on end of scope
  static unsigned char prevState;
  unsigned char state;

  // Delay for 1ms to avoid switch bounce
  __delay_ms(1);

  // Read the pin status from `REA` and `REB`, cast to correct data type
  state = (unsigned char)(REB << 1 | REA);

  // Check if state changed, if so increase/decrease `counter` accordingly
  if (prevState == 0) {
    if (state == ROTATE_LEFT) (*counter)++;
    else if (state == ROTATE_RIGHT) (*counter)--;
  }

  // Loop around if at max/min
  if (*counter > max) *counter = min;
  else if (*counter < min) *counter = max;

  // Update `prevState` for next call,
  // write to `PORTB` avoids a mismatch condition
  prevState = state;
  PORTB = PORTB;
}
