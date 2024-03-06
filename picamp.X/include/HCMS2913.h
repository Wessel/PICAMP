#ifndef HCMS2913_H
#define HCMS2913_H

#include "./HCMS2913.h"
#include "./config.h"

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

void HCMS_init(volatile unsigned char *port, uint8_t _RESET, uint8_t _CENABLE, uint8_t _RSELECT);
void HCMS_load_word(const char *_string, char *bufferSelected, volatile unsigned char *port, uint8_t _CENABLE);
void HCMS_spi_init(sspmode mst_slv_oscdiv, rxsmpmoment sdi_rx_smp, clkpol clock_pol, txclkedge sdo_tx_edge);

#endif