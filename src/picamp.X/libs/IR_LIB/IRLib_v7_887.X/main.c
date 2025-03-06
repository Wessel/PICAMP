/***********************************************************************
 * PIC   : 16F887
 * Date  : Between now and then
 **********************************************************************/

#include <xc.h>
#include <pic16f887.h>
#include <IRLib_v7.h>

#pragma config LVP = OFF // low voltage programming off
#pragma config FCMEN = OFF // fail-safe clock monitor off
#pragma config IESO = OFF // internal/external switchover off
#pragma config BOREN = ON // brown out reset on
#pragma config CPD = OFF //data code protection off
#pragma config CP = OFF // code protection off
#pragma config MCLRE = OFF // RE3/~MCLR is i/o pin
#pragma config PWRTE = ON // power up timer on
#pragma config WDTE = OFF //watchdog timer off
#pragma config FOSC = INTRC_NOCLKOUT //internal clock with io on RA6 and RA7

#pragma config WRT = OFF // write protection off
#pragma config BOR4V = BOR40V // brown out reset 4v

/* Hardware / state naming scheme ------------------------------------*/
#define LED1 PORTAbits.RA0
#define LED2 PORTAbits.RA1
#define LED3 PORTAbits.RA2
#define LED4 PORTAbits.RA3

//constants
#define _XTAL_FREQ      8000000  // Used by the XC8 delay_ms(x) macro
#define _MTPR 211
#define _MTPrSc 0b01
#define _MTPoSc 0b0001
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
    IRBind887();
    IRInit(falling, _MTPR, _MTPrSc, _MTPoSc, _TTPrSc);
}

void init_osc(void) {
    OSCCONbits.IRCF = 0b111; //111 8MHz
    OSCCONbits.OSTS = 0; // running from internal oscillator
    OSCCONbits.SCS = 0; //system clock selected by config
    while (OSCCONbits.HTS == 0);
}

void init_gpio(void) {
    TRISA = 0;
    ANSEL = 0;
    PORTA = 15;
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