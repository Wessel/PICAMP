#include "./IRLib_v7.h"

void IRBind887(void) {
    IRR.address = (unsigned char*) 0x006;
    IRR.offset = 0;
    IRR.size = 1;

    MTimer.address = (unsigned char*) 0x011;
    MTimer.offset = 0;
    MTimer.size = 8;

    MTOn.address = (unsigned char*) 0x012;
    MTOn.offset = 2;
    MTOn.size = 1;

    MTIF.address = (unsigned char*) 0x00C;
    MTIF.offset = 1;
    MTIF.size = 1;

    MTPR.address = (unsigned char*) 0x092;
    MTPR.offset = 0;
    MTPR.size = 8;

    MTPrSc.address = (unsigned char*) 0x012;
    MTPrSc.offset = 0;
    MTPrSc.size = 2;

    MTPoSc.address = (unsigned char*) 0x012;
    MTPoSc.offset = 3;
    MTPoSc.size = 4;

    TTimer.address = (unsigned char*) 0x00E;
    TTimer.offset = 0;
    TTimer.size = 8;

    TTOn.address = (unsigned char*) 0x010;
    TTOn.offset = 0;
    TTOn.size = 1;

    TTIF.address = (unsigned char*) 0x00C;
    TTIF.offset = 0;
    TTIF.size = 1;

    TTPrSc.address = (unsigned char*) 0x010;
    TTPrSc.offset = 4;
    TTPrSc.size = 2;

    IntPinTrisReg.address = (unsigned char*) 0x086;
    IntPinTrisReg.offset = 0;
    IntPinTrisReg.size = 1;

    IntPinAnselReg.address = (unsigned char*) 0x189;
    IntPinAnselReg.offset = 4;
    IntPinAnselReg.size = 1;

    IntPinIF.address = (unsigned char*) 0x00B;
    IntPinIF.offset = 1;
    IntPinIF.size = 1;

    IntEdgeReg.address = (unsigned char*) 0x081;
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

    PIEMTIE.address = (unsigned char*) 0x08C;
    PIEMTIE.offset = 1;
    PIEMTIE.size = 1;

    PIETTIE.address = (unsigned char*) 0x08C;
    PIETTIE.offset = 0;
    PIETTIE.size = 1;
}