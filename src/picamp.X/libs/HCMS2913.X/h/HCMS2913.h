#ifndef HCMS2913_H
#define HCMS2913_H

#include "HCMS2913.h"
#include <stdbool.h>
#include <stdint.h>

/* MANUAL:
 * 1/3 FILL IN HCMS_SETTINGS STRUCT
 * 2/3 RUN HCMS_spi_init()
 * 3/3 RUN HCMS_init()
 * HCMS_load_word AND HCMS_change_brightness CAN BE USED NOW.
 */
//!!Remember to set the Tristates to Output(0) for RSELECT, CENABLE, BLANK, RESET. And start the Port pins at 0. Dont forget your analog selection.
//FIGURE 1 ON PAGE 12 OF THE DATASHEET EXPLAINS EXACTLY WHY IT IS NECESSARY TO USE SPI MODE 0 (CKP0 CKE1). DATA IS LATCHED ON FALLING CLK WHILE nCE IS HIGH. 

typedef struct {
    volatile unsigned char * REGaddress;
    unsigned char pin;
} REGbit;

typedef struct {
    int s_insNeededFor200ns;
    //spi buffer
    volatile unsigned char * s_SSPBUF;
    REGbit s_SSPMlsb; //assumption: SSPM is always 4 bits long
    REGbit s_SSPBF;
    //spi data
    REGbit s_SCK_PIN;
    REGbit s_SDI_DDIR;
    REGbit s_SDO_DDIR;
    REGbit s_SCK_DDIR;
    //spi int
    REGbit s_SSPINTE;
    REGbit s_SSPINTFLAG;
    //spi settings
    REGbit s_CKP;
    REGbit s_CKE;
    REGbit s_SSPEN;
    REGbit s_SMP;
    //screen pins
    REGbit s_RESETpin;
    REGbit s_BLANKpin;
    REGbit s_CENABLEpin;
    REGbit s_RSELECTpin;
} HCMS_settings;

// PWM works on normal 4 bit binary: 0%, 1.7%, 3.3%, 5%, 6.7%, 8.3%, 11.7%, 15%, 18%, 23%, 30%, 37%, 47%, 60%, 80%, 100% 
// Peak Current Brightness Control: 00 = 73%,  01 = 50%, 10 = 31%, 11 = 100%
// Sleep: 1 = Normal operation, 0 = Disables internal Oscillator-display blank

typedef struct {
    bool PWM0, PWM1, PWM2, PWM3, PKBC0, PKBC1, SLEEP, WORDSELECT;
} HCMS_ControlWordZero;

//Serial/Simultaneous Data Out: 0 = D out holds contents of Bit D7, 1 = D out is functionally tied to Din.
//External Display Oscillator Prescaler: 0 = 1, 1 = 8.
//D2 - D6: Reserved for Future Use (Bits D 2 -D6 must be set Low)

typedef struct {
    bool SSDO, EXDO, D2, D3, D4, D5, D6, WORDSELECT;
} HCMS_ControlWordOne;

//these are loaded MSB first, make sure your code reflects this!
//HCMS_ControlWordZero HCMS2913_word0 = {.PWM0 = 0, .PWM1 = 0, .PWM2 = 0, .PWM3 = 0, .PKBC0 = 0, .PKBC1 = 0, .SLEEP = 1, .WORDSELECT = 0};
//HCMS_ControlWordOne HCMS2913_word1 = {.SSDO = 0, .EXDO = 0, .D2 = 0, .D3 = 0, .D4 = 0, .D5 = 0, .D6 = 0, .WORDSELECT = 1};

typedef enum {
    MST_OSC_DIV_04, // 0x0: 0000
    MST_OSC_DIV_16, // 0x1: 0001
    MST_OSC_DIV_64, // 0x2: 0010
    MST_OSC_DIV_T2, // 0x3: 0011
    SLV_SS_EN, // 0x4: 0100 - not used in this version of our lib ...
    SLV_SS_DIS // 0x5: 0101
} sspmode;

// Datasheet SMP (Sample Timing) behavior in (SPI MASTER MODE ONLY):
// 1 = Input data sampled (received) at end of data output time
// 0 = Input data sampled (recieved) at middle of data output time

typedef enum {
    SMP0_SDI_MIDDLE, SMP1_SDI_END
} rxsmpmoment;

// Datasheet CKP (Clock Polarity) select bit behavior (SPI mode):
// 1 = Idle state for clock is a high level
// 0 = Idle state for clock is a low level

typedef enum {
    CKP0_CPOL0_IDLE_LOW, CKP1_CPOL1_IDLE_HIGH
} clkpol;

// Datasheet CKE (Clock Edge) select bit behavior (SPI mode):
// 1 = Transmit occurs on transition from active to Idle clock state
// 0 = Transmit occurs on transition from Idle to active clock state
// ! NOTE: Microchip CKE bit is always the inverse of Motorola CPHA bit!

typedef enum {
    CKE0_CPHA1_TRSMIT_IDL2ACT, CKE1_CPHA0_TRSMIT_ACT2IDL
} txclkedge;

void HCMS_spi_init(sspmode mst_slv_oscdiv, rxsmpmoment sdi_rx_smp, clkpol clock_pol, txclkedge sdo_tx_edge, const HCMS_settings* _settings);
void HCMS_init(const HCMS_settings* _settings);
void HCMS_load_word(const char *_string, const HCMS_settings* _settings);
void HCMS_change_brightness(uint8_t setTo, const HCMS_settings* _settings);
#endif
