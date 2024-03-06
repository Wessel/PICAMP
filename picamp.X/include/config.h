#ifndef CONFIG_H
#define CONFIG_H

// PIC hardware mapping
#include <xc.h>

// PIC16F887 Configuration Bit Settings
// #pragma config statements should precede project file includes.
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF            // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON            // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF           // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF              // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF             // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON            // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF            // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF           // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF             // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
#pragma config BOR4V = BOR21V        // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF             // Flash Program Memory Self Write Enable bits (Write protection off)

// Used by the XC8 delay_ms(x) macro
#define _XTAL_FREQ 500000

#define REA PORTBbits.RB4
#define REB PORTBbits.RB5

#define NPN
#define PIC887A

#define ENABLED 1
#define DISABLED 0

#define LED_AMOUNT 4
#define POT_STEP_AMOUNT 10

#define POT_LIMIT 1023
#define DEADZONE 15

#define SCK_PIN                 PORTCbits.RC3
#define SDO_DDIR                TRISCbits.TRISC5 // DDIR = Data DIRection
#define SDI_DDIR                TRISCbits.TRISC4
#define SCK_DDIR                TRISCbits.TRISC3
#define SSPM                    SSPCONbits.SSPM

char selLoadString[8] = "SEL.  00";
char volLoadString[8] = "VOL. 000";
char volumeBuffer[40];
char selectBuffer[40];


//screen 1, right side
#define RESET_1   PORTCbits.RC2
#define CENABLE_1 PORTCbits.RC4 
#define BLANK_1   PORTCbits.RC6
#define RSELECT_1 PORTCbits.RC7

#define RESET_PINNUM   2
#define CENABLE_PINNUM 4 
#define BLANK_PINNUM   6
#define RSELECT_PINNUM 7

//screen 2, left side
#define RESET_2   PORTDbits.RD2
#define CENABLE_2 PORTDbits.RD4 
#define BLANK_2   PORTDbits.RD6
#define RSELECT_2 PORTDbits.RD7

#endif
