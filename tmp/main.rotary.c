/* main.c */
// A basic program to switch between four leds based on the state of a
// rotary encoder

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
#define REA PORTBbits.RB4
#define REB PORTBbits.RB5

// Function declarations
void pic_init(void);
void init_osc(void);
void init_gpio(void);
void pic_loop(void);

void main(void) {
  // Do all initialisation here
  pic_init();

  // Start indefinite program loop
  pic_loop();
}

void pic_loop(void) {
  int counter = 0;
  int counterOdd = 0;
  int statePrevious = REA;

  while (1) {
    // Compare `state` with `statePrevious`
    // Always increase `counterOdd` if `counter` is odd
    if (REA != statePrevious) {
      // Encoder rotates clockwise If `REB` is not `REA`,
      if (REB != REA) {
        counter++;
        if (counter % 2 != 0) counterOdd++;
      }

      // Encoder rotates counter clockwise If `REA` stays the same,
      // Limit `counterOdd` and `counter` to its max if it goes negative,
      // to prevent non exisiting input selection
      else {
        counter--;

        if (counter % 2 != 0) counterOdd--;
        if (counterOdd < 0) {
          counterOdd = LED_AMOUNT - 1;
          counter = LED_AMOUNT + LED_AMOUNT - 1;
        }
      }
    }

    // Update `statePrevious` to the current state of `REA`
    // (rotary encoder pin A), turn on/off the LEDs corresponding
    // to the selected input
    statePrevious = REA;
    PORTA = ~(1 << counterOdd);

    // Reset from `0` if `counterOdd` goes over the input (LED) limit
    if (counterOdd > LED_AMOUNT - 1) {
      counterOdd = 0;
      counter = 0;
    }
  }

  __delay_us(5);
}

void pic_init(void) {
  init_osc();
  init_gpio();
}

void init_gpio(void) {
  // A line = output line
  TRISA = 0;

  // B4 = rotary encoder pin `A`, set as input
  // B5 = rotary encoder pin `B`, set as input
  TRISBbits.TRISB4 = 1;
  TRISBbits.TRISB5 = 1;

  // 8-13 = digital, set to 0
  ANSELH = 0;

  // turn off all LEDs, `0` = off instead of `1` due to the use of transistors
  PORTA = 0b1111;
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
