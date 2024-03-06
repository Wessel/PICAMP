/* main.c */
// Include all global config variables
#include "../include/config.h"

// Include library header files
#include "../include/initialisation.h"
#include "../include/rotary_encoder.h"
#include "../include/potentiometer.h"

// Rotary encoder position
char count = 0;

// Function declarations
void pic_init(void);
void pic_loop(void);

void main(void) {
  // Do all initialisation here
  pic_init();
  // Start indefinite program loop
  pic_loop();
}

void pic_loop(void) {
  // todo(burhan): WRITE_TO_LED(VOL: (read_pot() * 100) / 1023);
  // todo: Remove code for LED showing after screen usage;
  // Initialize potentiometer step array
  int stepArray[LED_AMOUNT] = {0};
  populate_step_array(stepArray, LED_AMOUNT, POT_LIMIT);

  while (1) {
    if (count == 0) {
      // Parse result from potentiometer
      int result = read_pot();

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
    }
  }
}

void pic_init(void) {
  OscillatorConfig osc_config = { INTERNAL_CLK, KHZ500, INTERNAL_FOSC };
  init_osc(osc_config);

  PinConfig pin_config[] = { { &TRISA, &PORTA, &ANSEL, OUTPUT, DIGITAL } };
  init_gpio(pin_config, 1);

  // Moved to rotary_encoder.c
  // InterruptConfig int_config  = { GIE_ENABLED, EINT_ENABLED, PEIE_ENABLED,
  //                               T0INT_ENABLED, RBINT_ENABLED, FALLING_EDGE };
  // init_int(int_config);

  init_rotary(TRISBbits.TRISB4, TRISBbits.TRISB5, IOCB4, IOCB5);
  init_pot(ADC_AN07, TRISEbits.TRISE2);
}

// #[routine:interrupt_service]
void __interrupt() isr(void) {
  if (INTCONbits.RBIF == INT_OCCURED) {
    // Parse rotary changes into `count`, shift it into `PORTA`
    parse_rotary(REA, REB, &count, LED_AMOUNT, 0);

    // todo(burhan): WRITE_TO_LED(IN: count);
    // todo: Remove code for LED showing after screen usage;
    PORTA = (unsigned char) (~(1 << count));

    // Clear the Port B change interrupt flag
    INTCONbits.RBIF = INT_AWAITING;
    INTCONbits.INTF = INT_AWAITING;
  }
}
