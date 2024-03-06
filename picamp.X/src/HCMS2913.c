#include "../include/HCMS2913.h"
#include "../include/ascii.h"
#include "../include/config.h"

void HCMS_spi_init(sspmode mst_slv_oscdiv, rxsmpmoment sdi_rx_smp, clkpol clock_pol, txclkedge sdo_tx_edge) {
    //Todo Burhan: remove this dirty fix
    TRISC = 0;
    TRISD = 0;
    PORTC = 0;
    PORTD = 0;
    // 1. Set `SSPM' datafield (SPI master / slave mode and prescaler 
    //		selection)
    SSPM = mst_slv_oscdiv;
    // 2. Set SDO_DDIR for SDO to behave as an output
    SDO_DDIR = 0;
    SDI_DDIR = 0;
    // 3. If a slave mode was selected:
    //		3a. The SMP bit MUST remain clear (see datasheet)
    //		3b. Set SCK_DDIR for SCK to behave like an input
    //		3c. The data line `idle state' (CKP bit) must match the 
    //			master's clock idle state. To achieve this, set the 
    //			CKP bit to whatever value can be read on the SCK_PIN.
    //		3d. Set up the recieve interrupt (PIE1.SSPIE). Don't
    //			forget PEIE and GIE (interrupt masking hierarchy).
    if (mst_slv_oscdiv > 3) {
        SMP = 0;
        SCK_DDIR = 1;
        CKP = SCK_PIN;
    }// 4. Else (if a master mode was selected):
        //		4a. The SMP bit needs to be set according to argument 
        //			`sdi_rx_smp'
        //		4b. Set SCK_DDIR for SCK to behave like an output
        //		4c. The data line `idle state' (CKP bit) needs to be set
        //			according to argument `clock_pol'
    else {
        SMP = sdi_rx_smp;
        SCK_DDIR = 0;
        CKP = clock_pol;
    }
    //SSPIE = 1;
    // 5. Set the CKE bit according to the `sdo_tx_edge' and `clock_pol'
    //		parameters (i.e. if `sdo_tx_edge' and `clock_pol' select 
    //		mode 0 (CKE 1, CKP 0), then the CKE bit must be set, etc. -
    //		(handle all SPI modes ... !)
    CKE = sdo_tx_edge;
    if (clock_pol == 1 || SCK_PIN == 1) {
        CKE ^= 1;
    }
    // 6. Enable the SPI module (by setting SSPEN bit).
    SSPEN = 1;
}

void delayNanoSeconds(unsigned long nanoSeconds);

#define BUFFER                  SSPBUF

void spi_transmit(char c) {
    // FILL. THAT. BUFFER.
    BUFFER = c;
}

void setpin(volatile unsigned char *port, unsigned char pin, int value) {
    if (value == 0)
        *port &= ~(1 << pin);
    else
        *port |= 1 << pin;
}

void HCMS_init(volatile unsigned char *port, uint8_t _RESET, uint8_t _CENABLE, uint8_t _RSELECT) {
    setpin(port, _RESET, 0);
    delayNanoSeconds(200);
    setpin(port, _RESET, 1);

    setpin(port, _RSELECT, 0);
    setpin(port, _CENABLE, 0);
    for (int i = 0; i < 40; i++) {
        spi_transmit(0b00000000);
        delayNanoSeconds(200);
    }
    setpin(port, _CENABLE, 1);

    setpin(port, _RSELECT, 1);
    setpin(port, _CENABLE, 0);
    spi_transmit(0b01111111);
    delayNanoSeconds(200);
    setpin(port, _CENABLE, 1);
    setpin(port, _CENABLE, 0);
    spi_transmit(0b10000000);
    delayNanoSeconds(200);
    setpin(port, _CENABLE, 1);

    setpin(port, _CENABLE, 0);
    spi_transmit(0b01111111);
    delayNanoSeconds(200);
    setpin(port, _CENABLE, 1);
    setpin(port, _CENABLE, 0);
    spi_transmit(0b10000000);
    delayNanoSeconds(200);
    setpin(port, _CENABLE, 1);

    setpin(port, _RSELECT, 0);

}

void load_buffer(int character, int position, char *bufferSelected) {
    for (int i = 0; i < 5; i++) {
        bufferSelected[position * 5 + i] = ascii[character][i];
    }
}

int TableLookup(char lookup) {
    for (int i = 0; i < 128; ++i) {
        if (ascii[i][5] == lookup) {
            return i;
        }
    }
    return 127; // Character not found in the ASCII table
}

void HCMS_load_word(const char *_string, char *bufferSelected, volatile unsigned char *port, uint8_t _CENABLE) {
    short length = 0;
    setpin(port, _CENABLE, 0);

    while (_string[length] != '\0' && length < 8) {
        load_buffer(TableLookup(_string[length]), length, bufferSelected);
        length++;
    }

    while (length < 8) {
        load_buffer(TableLookup(' '), length, bufferSelected);
        length++;
    }

    for (int i = 0; i < 40; i++) {
        spi_transmit(bufferSelected[i]);
    }

    delayNanoSeconds(200);
    setpin(port, _CENABLE, 1);
}

void delayNanoSeconds(unsigned long nanoSeconds) {
    const unsigned long cyclesPerNanoSecond = 10;

    unsigned long cyclesToDelay = nanoSeconds * cyclesPerNanoSecond;

    TMR1 = 0;

    while (cyclesToDelay > 0) {
        cyclesToDelay--;
    }
}
