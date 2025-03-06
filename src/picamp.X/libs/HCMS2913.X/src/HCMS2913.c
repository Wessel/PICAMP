#include "./../h/HCMS2913.h"
#include "./../h/HCMS_ASCII.h"
#include "./../h/HCMS_brightness.h"

//To do: fix types in HCMS_SETTINGS SO PINS CAN BE PASSED TO THE STRUCT

const uint8_t init_word0 = 0b01111111, init_word1 = 0b10000000;
// 1/3.
//HCMS_SETTINGS = {};

void HCMS_delay200ns(const HCMS_settings* _settings);

void HCMS_setbitinreg(REGbit a, short value) {
    //uint8_t mask = ~(1 << a.pin); 0b11111110
    if (value == 0)
        *a.REGaddress &= ~(1 << a.pin);
    else
        *a.REGaddress |= 1 << a.pin;
}

bool HCMS_getbitinreg(REGbit a) {
    //uint8_t mask = 1 << _bit.pin; 0b00000001
    return (*a.REGaddress & (1 << a.pin)) >> a.pin;
}

// 2/3.

void HCMS_spi_init(sspmode mst_slv_oscdiv, rxsmpmoment sdi_rx_smp, clkpol clock_pol, txclkedge sdo_tx_edge, const HCMS_settings* _settings) {
    *_settings->s_SSPMlsb.REGaddress &= ~(mst_slv_oscdiv << _settings->s_SSPMlsb.pin);

    HCMS_setbitinreg(_settings->s_SDO_DDIR, 0);

    if (mst_slv_oscdiv & SLV_SS_EN || mst_slv_oscdiv & SLV_SS_DIS) { //SPI SLAVE
        HCMS_setbitinreg(_settings->s_SMP, 0);
        HCMS_setbitinreg(_settings->s_SCK_DDIR, 1);
        HCMS_setbitinreg(_settings->s_CKP, HCMS_getbitinreg(_settings->s_SCK_PIN));
        HCMS_setbitinreg(_settings->s_SSPINTE, 1);
    } else { //SPI MASTER
        HCMS_setbitinreg(_settings->s_SMP, sdi_rx_smp);
        HCMS_setbitinreg(_settings->s_SCK_DDIR, 0);
        HCMS_setbitinreg(_settings->s_CKP, clock_pol);
    }

    //    CKE = ~(clock_pol ^ sdo_tx_edge); //XNOR
    HCMS_setbitinreg(_settings->s_CKE, sdo_tx_edge);
    if (clock_pol == 1 || HCMS_getbitinreg(_settings->s_SCK_PIN)) {
        HCMS_setbitinreg(_settings->s_CKE, 1);
    }
    HCMS_setbitinreg(_settings->s_SSPEN, 1);
}

void spi_transmit(char c, volatile unsigned char * _buffer) {
    *_buffer = c;
}

// 3/3.

void HCMS_init(const HCMS_settings* _settings) {
    //reset
    HCMS_setbitinreg(_settings->s_RESETpin, 0);
    HCMS_delay200ns(_settings);
    HCMS_setbitinreg(_settings->s_RESETpin, 1);

    //flush register
    HCMS_setbitinreg(_settings->s_RSELECTpin, 0);
    HCMS_setbitinreg(_settings->s_CENABLEpin, 0);
    for (int i = 0; i < 40; i++) {
        spi_transmit(0x00, _settings->s_SSPBUF);
        HCMS_delay200ns(_settings);
    }
    HCMS_setbitinreg(_settings->s_CENABLEpin, 1);
    HCMS_setbitinreg(_settings->s_RSELECTpin, 1);

    HCMS_setbitinreg(_settings->s_CENABLEpin, 0);
    spi_transmit(init_word0, _settings->s_SSPBUF);
    HCMS_delay200ns(_settings);
    HCMS_setbitinreg(_settings->s_CENABLEpin, 1);
    HCMS_setbitinreg(_settings->s_CENABLEpin, 0);
    spi_transmit(init_word1, _settings->s_SSPBUF);
    HCMS_delay200ns(_settings);
    HCMS_setbitinreg(_settings->s_CENABLEpin, 1);

    HCMS_setbitinreg(_settings->s_RSELECTpin, 0);
}

//set brightness in control register 0

void HCMS_change_brightness(uint8_t setTo, const HCMS_settings* _settings) {
    //unsigned so lower bound by nature
    if (setTo > HCMS2913_BRIGHTNESS_SETTINGS_COUNT - 1) {
//if too high, set to 100%
        setTo = HCMS2913_BRIGHTNESS_SETTINGS_COUNT - 1;
    }

    uint8_t message = 0b01000000;
    message = HCMS_BRIGHT_VALUES[setTo];

    HCMS_setbitinreg(_settings->s_RSELECTpin, 1);

    HCMS_setbitinreg(_settings->s_CENABLEpin, 0);
    spi_transmit(message, _settings->s_SSPBUF);
    HCMS_delay200ns(_settings);
    HCMS_setbitinreg(_settings->s_CENABLEpin, 1);
    HCMS_setbitinreg(_settings->s_CENABLEpin, 0);
    spi_transmit(message, _settings->s_SSPBUF);
    HCMS_delay200ns(_settings);
    HCMS_setbitinreg(_settings->s_CENABLEpin, 1);

    HCMS_setbitinreg(_settings->s_RSELECTpin, 0);
}

//void load_buffer(int character, int position, char *bufferSelected) {
//    for (int i = 0; i < 5; i++) {
//        bufferSelected[position * 5 + i] = HCMS_ascii[character][i];
//    }
//}

short TableLookup(char lookup) {
    for (int i = 0; i < ASCII_TABLE_SIZE; ++i) {
        if (HCMS_ascii[i][5] == lookup) {
            return i;
        }
    }
    return ASCII_TABLE_SIZE - 1; // Character not found in the ASCII table
}

void HCMS_load_word(const char *_string, const HCMS_settings* _settings) {
    short character = ASCII_TABLE_SIZE - 1;
    char letterRow = ' ';

    HCMS_setbitinreg(_settings->s_CENABLEpin, 0);
    for (short i = 0; i < 8; i++) {
        //display character loop. send letter out per character
        //check where on the table the current character is
        character = TableLookup(_string[i]);
        for (short j = 0; j < 5; j++) {
            //column loop, fill letter with lookup table
            letterRow = HCMS_ascii[character][j];
            spi_transmit(letterRow, _settings->s_SSPBUF);
        }
    }

    //    while (_string[length] != '\0' && length < 8) {
    //        load_buffer(TableLookup(_string[length]), length, bufferVar);
    //        length++;
    //    }
    //
    //    while (length < 8) {
    //        load_buffer(TableLookup(' '), length, bufferVar);
    //        length++;
    //    }
    //
    //    for (int i = 0; i < 40; i++) {
    //        spi_transmit(bufferVar[i], _settings.s_SSPBUF);
    //    }

    HCMS_delay200ns(_settings);
    HCMS_setbitinreg(_settings->s_CENABLEpin, 1);
}

void HCMS_delay200ns(const HCMS_settings* _settings) {
    //    // 1s / 10MHz = 10ns! All that is required it to get a ratio!
    //    float cyclesRequired = _settings.s_Timer_Frequency / 10000000;
    //    if (cyclesRequired < 1.0f) cyclesRequired = 1.0f;
    //    unsigned char * yes = _ticks * (_settings.s_Timer_MaxValue - (unsigned char) cyclesRequired);
    //    _settings.s_Timer_REG = yes;
    //
    //    while (HCMS_getbitinreg(_settings.s_Timer_INTF)) {
    //    }
    //    HCMS_setbitinreg(_settings.s_Timer_INTF, 0); // set INTF to 0
    //timer not needed, most PICs have an instruction speed very close to 200ns
    int time = 1 + _settings->s_insNeededFor200ns;

    while (time > 0) {
        time--;
    }
}