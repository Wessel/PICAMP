#ifndef CONFIG_H
#define CONFIG_H

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
#define _XTAL_FREQ 8000000

#define LED_AMOUNT 4

#define POT_LIMIT 1023
#define DEADZONE 2

#include <xc.h>
#include "./libs/HCMS2913.X/h/HCMS2913.h"
#include "./libs/global/picamp-potentiometer.h"
#include "./libs/global/picamp-rotary.h"
#include "./libs/IR_LIB/IRLib_v7.X/IRLib_v7.h"

//P moves right, M moves left
#define MOTOR_P PORTCbits.RC1
#define MOTOR_Ptris TRISCbits.TRISC1
#define MOTOR_M PORTCbits.RC0
#define MOTOR_Mtris TRISCbits.TRISC0

#define LED1 PORTAbits.RA0
#define LED2 PORTAbits.RA1
#define LED3 PORTAbits.RA2
#define LED4 PORTAbits.RA3

char selLoadString[8] = "SEL.  00";
char volLoadString[8] = "VOL. 000";

//The screen on the right uses portc
const HCMS_settings screenSettings_1 = {
    1,
    //spi buffer
    &SSPBUF,
    {&SSPCON, 0},
    {&SSPSTAT, 0},
    //spi data
    {&PORTC, 3},
    {&TRISC, 4}, //SDI
    {&TRISC, 5},
    {&TRISC, 3},
    //spi int
    {&PIE1, 3},
    {&PIR1, 3},
    //spi settings
    {&SSPCON, 4},
    {&SSPSTAT, 6},
    {&SSPCON, 5},
    {&SSPSTAT, 7},
    //screen pins
    {&PORTC, 2},
    {&PORTC, 6},
    {&PORTC, 4},
    {&PORTC, 7}
};

const HCMS_settings screenSettings_2 = {
    1,
    //spi buffer
    &SSPBUF,
    {&SSPCON, 0},
    {&SSPSTAT, 0},
    //spi data
    {&PORTC, 3},
    {&TRISC, 4}, //SDI
    {&TRISC, 5},
    {&TRISC, 3},
    //spi int
    {&PIE1, 3},
    {&PIR1, 3},
    //spi settings
    {&SSPCON, 4},
    {&SSPSTAT, 6},
    {&SSPCON, 5},
    {&SSPSTAT, 7},
    //screen pins
    {&PORTD, 2},
    {&PORTD, 6},
    {&PORTD, 4},
    {&PORTD, 7}
};

#define _MTPR 211
#define _MTPrSc 0b01
#define _MTPoSc 0b0001
#define _TTPrSc 0b11
#endif
