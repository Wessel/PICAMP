/* main.c */
// A basic program to switch between four leds based on the state of a
// potmeter

// PIC hardware mapping
#include <xc.h>

// PIC16F887 Configuration Bit Settings
#define _XTAL_FREQ      500000  // Used by the XC8 delay_ms(x) macro
// 'C' source line config statements

// CONFIG1
#pragma config FOSC  = INTRC_NOCLKOUT // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE  = OFF            // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON             // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF            // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP    = OFF            // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD   = OFF            // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON             // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO  = OFF            // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP   = OFF            // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define LED_AMOUNT 4
#define POT_LIMIT 1023
#define DEADZONE 2

// Function declarations
void pic_init(void);
void init_osc(void);
void init_gpio(void);
void pic_loop(int *PotSteps);

void main(void) {
  // Do all initialisation here
  pic_init();

  int PotSteps[LED_AMOUNT];
  PotSteps[0] = POT_LIMIT / (LED_AMOUNT + 1);
  for (int i = 0; i < LED_AMOUNT; i++) {
    PotSteps[i] = PotSteps[0] * (i + 1);
  }

  // Start indefinite program loop
  pic_loop(PotSteps);
}

void pic_loop(int *PotSteps) {
  while (1) {
    // Wait for regulating capacitor to charge
    __delay_us(5);

    // Bit 2 of `ADCON0` is set to `1`, meaning `GO` will wait until
    // ADC got a signal, converted analog to digital, etc.
    GO = 1;
    while (GO) continue;

    // Read the data from the potmeter pin (`E2`) registers, and combine
    // them into a single integer
//    int result = ADRESH;
//    result = result << 8;
//    result = result + ADRESL;
    int result = (ADRESH * 256) + ADRESL;

    // Loop through all `PotSteps`, check if the output of the potmeter
    // (on `ADRES`) falls inbetween the steps - `DEADZONE`, if so execute
    // An OR/AND bitshift operation in order to turn on the corresponding
    // LED on `PORTA`.
    for (int i = 0; i < LED_AMOUNT; i++) {
      if (result > (PotSteps[i] - DEADZONE)) {
        PORTA = (unsigned char)(PORTA & ~(1 << i));
      } else if (result <= (PotSteps[i] - DEADZONE)) {
        PORTA = (unsigned char)(PORTA | (1 << i));
      }
    }
  }
}

void pic_init(void) {
  init_osc();
  init_gpio();
}

void init_gpio(void) {
  // A line = output line
  TRISA = 0;

  // E2 = Potmeter pin, set as input
  TRISEbits.TRISE2 = 1;

  ANSEL = 0;
  ANSELH = 0;
  ANSELbits.ANS7 = 1;

  ADCON0 = 0b00011111;
  ADCON1bits.ADFM = 1; // right justified

  // turn off all LEDs, `0` = off instead of `1` due to the use of transistors
  // except the first LED
  PORTA = 1;
}

void init_osc(void) {
  // System Clock Select (SCS)
  OSCCONbits.SCS = 0b1; // Internal Clock selected
  // Internal Resistor-Capacitor Frequency select (IRCF)
  OSCCONbits.IRCF = 0b011; // 500 kHz clock speed
  OSCCONbits.OSTS = 0b1; // OSC startup time

  // Halt boot until clock is stable
  while (OSCCONbits.HTS != 0b1);
}
