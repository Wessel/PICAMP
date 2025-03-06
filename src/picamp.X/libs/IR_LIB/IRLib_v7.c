#include "./IRLib_v7.h"

void IRInit(IntEdge IntEdge, unsigned char vMTPR, unsigned char vPTPrSc, unsigned char vMTPoSc, unsigned char vTTPrSc) {
    //interrupts
    SetReg(&IntConGIE, 1);
    SetReg(&IntConINTE, 1);
    SetReg(&IntConPEIE, 1);

    //measure timer config
    SetReg(&MTPrSc, vPTPrSc);
    SetReg(&MTPR, vMTPR);
    SetReg(&MTPoSc, vMTPoSc);
    SetReg(&MTOn, 0);
    SetReg(&PIEMTIE, 1);

    //timeout timer
    SetReg(&TTPrSc, vTTPrSc);
    SetReg(&TTOn, 0);
    SetReg(&PIETTIE, 1);

    //int pin
    SetReg(&IntPinTrisReg, 1);
    SetReg(&IntPinAnselReg, 0);
    SetReg(&IntEdgeReg, IntEdge);
}

void IntHandler() {
    SetReg(&MTimer, 0);
    SetReg(&MTOn, 1);
    SetReg(&TTimer, 0);
    SetReg(&TTOn, 1);
    SetReg(&IntPinIF, 0);
}

void MTHandler() {
    dataBuffer = dataBuffer << 1;
    if (!ReadReg(&IRR)) {
        dataBuffer |= 0b1;
    }
    count++;
    SetReg(&MTOn, 0);
    if (count >= MessageSize) {

        data = dataBuffer;
        dataBuffer = 0;
        count = 0;
        IRBF = 1;
    }
    SetReg(&MTIF, 0);
}

void TTHandler() {
    count = 0;
    dataBuffer = 0;
    SetReg(&TTOn, 0);
    SetReg(&TTimer, 0);
    SetReg(&TTIF, 0);
}

void SetReg(Reg* R, unsigned char V) {
    unsigned char bitMask;
    V = (unsigned char) (V << R->offset);
    bitMask = (unsigned char) (((1 << R->size) - 1) << R->offset);
    *(R->address) &= ~bitMask;
    *(R->address) |= V & bitMask;
}

unsigned char ReadReg(Reg* R) {
    unsigned char V, bitMask;
    bitMask = (unsigned char) (((1 << R->size) - 1) << R->offset);
    V = *(R->address) & bitMask;
    V = V >> R->offset;
    return V;
}
