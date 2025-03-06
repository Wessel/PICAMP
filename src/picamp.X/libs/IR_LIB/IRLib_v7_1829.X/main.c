/***********************************************************************
 * PIC   : 16F1829
 * Date  : Between now and then
 **********************************************************************/

#include <xc.h>                 // PIC hardware mapping
#include <pic16f1829.h>
#include <IRLib_v7.h>

#pragma config FOSC     = 0x4   // xor INTOSC (name from IDE database)
#pragma config WDTE     = 0x0   // OFF
#pragma config PWRTE    = 0x1   // OFF
#pragma config MCLRE    = OFF   // etc.
#pragma config CP       = OFF   // text is the most readable option
#pragma config CPD      = OFF
#pragma config BOREN    = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO     = OFF
#pragma config FCMEN    = OFF

// CONFIG WORD 2 @ flash program memory location 0x8008
#pragma config WRT      = OFF
#pragma config PLLEN    = OFF
#pragma config STVREN   = OFF
#pragma config BORV     = LO
//#pragma config DEBUG    = OFF   // Pre-processor short-circuits !
#pragma config LVP      = OFF

/* Hardware / state naming scheme ------------------------------------*/
#define LED1 LATCbits.LATC0
#define LED2 LATCbits.LATC1
#define LED3 LATCbits.LATC2
#define LED4 LATCbits.LATC3

//constants
#define _XTAL_FREQ      16000000  // Used by the XC8 delay_ms(x) macro
#define _MTPR 211
#define _MTPrSc 0b01
#define _MTPoSc 0b0011
#define _TTPrSc 0b11

void pic_init(void);
void init_osc(void);
void init_gpio(void);

void main(void) {
    pic_init();
    while (1) {
        if (IRBF) {
            if (data == 0b010010010000) { //input 1
                LED1 = 0;
                __delay_ms(250);
                LED1 = 1;
            }
            if (data == 0b010100001000) { //input 2
                LED2 = 0;
                __delay_ms(250);
                LED2 = 1;
            }
            if (data == 0b010010100000) { //input 3
                LED3 = 0;
                __delay_ms(250);
                LED3 = 1;
            }
            if (data == 0b010010001000) { //input 4
                LED4 = 0;
                __delay_ms(250);
                LED4 = 1;
            }
            if (data == 0b010100100000) { //volume up
                LED1 = 0;
                LED2 = 0;
                __delay_ms(250);
                LED1 = 1;
                LED2 = 1;
            }
            if (data == 0b010100010000) { //volume down
                LED3 = 0;
                LED4 = 0;
                __delay_ms(250);
                LED3 = 1;
                LED4 = 1;
            }
            IRBF = 0;
        }
    }//while
}//main

void pic_init(void) {
    init_osc();
    init_gpio();
    IRBind1829();
    IRInit(falling, _MTPR, _MTPrSc, _MTPoSc, _TTPrSc);
}

void init_osc(void) {
    // System Clock Select (SCS)
    OSCCONbits.SCS = 0b00; // Clk determined by FOSC in Conf-1
    // Internal Resistor-Capacitor Frequency select (IRCF)
    OSCCONbits.IRCF = 0b1111; // 16 MHz clock speed
    // Software Phase-Locked Loop ENable (SPLLEN)
    OSCCONbits.SPLLEN = 0b0; // 4 x Phase-Locked Loop disabled
}

void init_gpio(void) {
    TRISC = 0;
    LATC = 15;
}

//interrupt service routine

void __interrupt() isr() {
    if (INTF) {
        IntHandler(); //Interrupt Handling
    }//INTF

    if (TMR2IF) {
        MTHandler(); //Measure Timer Handling
    }//TMR2IF

    if (TMR1IF) { //timeout / reset
        TTHandler(); //Timeout Timer Handling
    } //TMR1IF
}//interrupt