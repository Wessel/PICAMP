#include "./IRLib_v7.h"

void IRBind1829(void) {
    IRR.address = (unsigned char*) 0x00C;
    IRR.offset = 2;
    IRR.size = 1;

    MTimer.address = (unsigned char*) 0x01A;
    MTimer.offset = 0;
    MTimer.size = 8;

    MTOn.address = (unsigned char*) 0x01C;
    MTOn.offset = 2;
    MTOn.size = 1;

    MTIF.address = (unsigned char*) 0x011;
    MTIF.offset = 1;
    MTIF.size = 1;

    MTPR.address = (unsigned char*) 0x01B;
    MTPR.offset = 0;
    MTPR.size = 8;

    MTPrSc.address = (unsigned char*) 0x01C;
    MTPrSc.offset = 0;
    MTPrSc.size = 2;

    MTPoSc.address = (unsigned char*) 0x01C;
    MTPoSc.offset = 3;
    MTPoSc.size = 4;

    TTimer.address = (unsigned char*) 0x016;
    TTimer.offset = 0;
    TTimer.size = 8;

    TTOn.address = (unsigned char*) 0x018;
    TTOn.offset = 0;
    TTOn.size = 1;

    TTIF.address = (unsigned char*) 0x011;
    TTIF.offset = 0;
    TTIF.size = 1;

    TTPrSc.address = (unsigned char*) 0x018;
    TTPrSc.offset = 4;
    TTPrSc.size = 2;

    IntPinTrisReg.address = (unsigned char*) 0x08C;
    IntPinTrisReg.offset = 2;
    IntPinTrisReg.size = 1;

    IntPinAnselReg.address = (unsigned char*) 0x18C;
    IntPinAnselReg.offset = 2;
    IntPinAnselReg.size = 1;

    IntPinIF.address = (unsigned char*) 0x00B;
    IntPinIF.offset = 1;
    IntPinIF.size = 1;

    IntEdgeReg.address = (unsigned char*) 0x095;
    IntEdgeReg.offset = 6;
    IntEdgeReg.size = 1;

    IntConGIE.address = (unsigned char*) 0x00B;
    IntConGIE.offset = 7;
    IntConGIE.size = 1;

    IntConINTE.address = (unsigned char*) 0x00B;
    IntConINTE.offset = 4;
    IntConINTE.size = 1;

    IntConPEIE.address = (unsigned char*) 0x00B;
    IntConPEIE.offset = 6;
    IntConPEIE.size = 1;

    PIEMTIE.address = (unsigned char*) 0x091;
    PIEMTIE.offset = 1;
    PIEMTIE.size = 1;

    PIETTIE.address = (unsigned char*) 0x091;
    PIETTIE.offset = 0;
    PIETTIE.size = 1;
}