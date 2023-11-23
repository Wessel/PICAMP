// PIC hardware mapping, global config
#include "../include/config.h"

// Rotary encoder structs and enums
#include "../include/rotary_encoder.h"

void parse_rotary(char pinA, char pinB, char* counter, char max, char min) {
  // static values retain their state, even on end of scope
  static unsigned char prevState;
  unsigned char state;

  // Delay for 1ms to avoid switch bounce
  __delay_ms(1);

  // Read the pin status from `REA` and `REB`, cast to correct data type
  state = (unsigned char)(pinB << 1 | pinA);

  // Check if state changed, if so increase/decrease `counter` accordingly
  if (prevState == 0) {
    if (state == ROTATE_LEFT) (*counter)--;
    else if (state == ROTATE_RIGHT) (*counter)++;
  }

  // Loop around if at max/min
  if (*counter > max) *counter = min;
  else if (*counter < min) *counter = max;

  // Update `prevState` for next call,
  // write to `PORTB` avoids a mismatch condition
  prevState = state;
  PORTB = PORTB;
}
