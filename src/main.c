/* main.c */

#include "../include/config.h"

#include "../include/initialisation.h"
#include "../include/rotary_encoder.h"
#include "../include/potentiometer.h"

//todo(wessel): Remove global
char count = 0;

// Function declarations
void pic_init(void);
void init_gpio(void);

void pic_loop(void);

void main(void) {
  // Do all initialisation here
  pic_init();
  // Start indefinite program loop
  pic_loop();
}

void pic_loop(void) {
  // Initialize potentiometer step array
  int stepArray[LED_AMOUNT] = {0};
  populate_step_array(stepArray, LED_AMOUNT, POT_LIMIT);

  while (1) {
    if (count == 0) {
      // Parse result from potentiometer
      int result = read_potentiometer();

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
  ADCConfig adc_config        = { ADC_ON, ADC_RIGHT, ADC_AN07, ADC_FOSC02,
                                  VREF_PLUSPIN, VREF_MINPIN, ADC_IN_PROGRESS };
  OscillatorConfig osc_config = { INTERNAL_CLK, KHZ500, INTERNAL_FOSC };
  InterruptConfig int_config  = { GIE_ENABLED, EINT_DISABLED, PEIE_DISABLED,
                                T0INT_DISABLED, RBINT_ENABLED, RISING_EDGE };

  // PinConfig pin_config[] = {
  //   { &TRISA, &PORTA, &ANSEL, OUTPUT, DIGITAL }, // LEDs
  //   { &TRISB, &PORTB, &ANSELH, INPUT, DIGITAL }, // Rotary encoder A
  //   { &TRISBbits.TRISB5, &PORTBbits.RB5, &ANSELH,   INPUT, DIGITAL }, // Rotary encoder B
  //   { &TRISEbits.TRISE2, &PORTE, &ANSELH, INPUT, ANALOG } // Potmeter
  // };
  // init_gpio2(pin_config);

  init_osc(osc_config);
  init_int(int_config);
  init_adc(adc_config);
  init_gpio();
}

// #[initialisation:gpio]
// todo(wessel): Make init_gpio() from initialisation.c work
void init_gpio(void) {
  // LED => output
  TRISA = OUTPUT;
  PORTA = OFF;
  // Rotary => input
  TRISBbits.TRISB4 = INPUT;
  TRISBbits.TRISB5 = INPUT;
  // Potentiometer => input
  TRISEbits.TRISE2 = INPUT;
  // Set all pins to digital
  ANSEL = 0;
  ANSELH = 0;
  ANSELbits.ANS7 = 1;
}

// #[routine:interrupt_service]
void __interrupt() isr(void) {
  if (INTCONbits.RBIF == 1) {
    // Parse rotary changes into `count`, shift it into `PORTA`
    parse_rotary(REA, REB, &count, LED_AMOUNT, 0);
    PORTA = (unsigned char) (~(1 << count));

    // Clear the Port B change interrupt flag
    INTCONbits.RBIF = INT_AWAITING;
    // INTCONbits.INTF = INT_AWAITING;
  }
}
