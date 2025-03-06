#include "./IRLib_v7.h"

void IRInit(IntEdge IntEdge, unsigned char vMTPR, unsigned char vPTPrSc, unsigned char vMTPoSc, unsigned char vTTPrSc) {
    //interrupts
    IR_SetReg(&IntConGIE, 1);
    IR_SetReg(&IntConINTE, 1);
    IR_SetReg(&IntConPEIE, 1);

    //measure timer config
    IR_SetReg(&MTPrSc, vPTPrSc);
    IR_SetReg(&MTPR, vMTPR);
    IR_SetReg(&MTPoSc, vMTPoSc);
    IR_SetReg(&MTOn, 0);
    IR_SetReg(&PIEMTIE, 1);

    //timeout timer
    IR_SetReg(&TTPrSc, vTTPrSc);
    IR_SetReg(&TTOn, 0);
    IR_SetReg(&PIETTIE, 1);

    //int pin
    IR_SetReg(&IntPinTrisReg, 1);
    IR_SetReg(&IntPinAnselReg, 0);
    IR_SetReg(&IntEdgeReg, IntEdge);
}

void IntHandler() {
    IR_SetReg(&MTimer, 0);
    IR_SetReg(&MTOn, 1);
    IR_SetReg(&TTimer, 0);
    IR_SetReg(&TTOn, 1);
    IR_SetReg(&IntPinIF, 0);
}

void MTHandler() {
    dataBuffer = dataBuffer << 1;
    if (!ReadReg(&IRR)) {
        dataBuffer |= 0b1;
    }
    count++;
    IR_SetReg(&MTOn, 0);
    if (count >= MessageSize) {

        data = dataBuffer;
        dataBuffer = 0;
        count = 0;
        IRBF = 1;
    }
    IR_SetReg(&MTIF, 0);
}

void TTHandler() {
    count = 0;
    dataBuffer = 0;
    IR_SetReg(&TTOn, 0);
    IR_SetReg(&TTimer, 0);
    IR_SetReg(&TTIF, 0);
}

void IR_SetReg(IR_Reg* R, unsigned char V) {
    unsigned char bitMask;
    V = (unsigned char) (V << R->offset);
    bitMask = (unsigned char) (((1 << R->size) - 1) << R->offset);
    *(R->address) &= ~bitMask;
    *(R->address) |= V & bitMask;
}

unsigned char ReadReg(IR_Reg* R) {
    unsigned char V, bitMask;
    bitMask = (unsigned char) (((1 << R->size) - 1) << R->offset);
    V = *(R->address) & bitMask;
    V = V >> R->offset;
    return V;
}
