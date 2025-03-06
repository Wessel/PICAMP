#include "../global/picamp-rotary.h"
#include "../global/registers.h"

void init_rot(picId id) {
  switch (id) {
    case PIC16F1829: _rot_init_1829(); break;
    case PIC16F887A: _rot_init_887(IOCRB4, IOCRB5); break;
  }

  encoder.LastState = 1;
  encoder.Input = 1;
  encoder.min = 1;
  encoder.max = 4;
  encoder.stepReq = 1;
}


void _rot_init_887(IOCPin pinA, IOCPin pinB) {
  encoder.host = PIC16F887A;
  //todo: Add anselb, trisb
    SetRawReg((unsigned char*) 0x189, 1, 5, 0); // ANSEL.13
    SetRawReg((unsigned char*) 0x189, 1, 3, 0); // ANSEL.11
    SetRawReg((unsigned char*) 0x086, 1, 4, 1); // TRISB.4
    SetRawReg((unsigned char*) 0x086, 1, 5, 1); // TRISB.5

  encoder.encoderRail = (Reg) { (unsigned char*) 0x006, 8, 0 };

  SetRawReg((unsigned char*) 0x00B, 2, 3, 0b11); // RBIE, INTE
  SetRawReg((unsigned char*) 0x00B, 2, 6, 0b11); // GIE, PEIE
  SetRawReg((unsigned char*) 0x096, 1, pinA, 1); // IOC Pin A
  SetRawReg((unsigned char*) 0x096, 1, pinB, 1); // IOC Pin B
  SetRawReg((unsigned char*) 0x081, 1, 6, 0); // INTEDG

  encoder.pinA = (Reg) { (unsigned char*) 0x006, 1, 4 };
  encoder.pinB = (Reg) { (unsigned char*) 0x006, 1, 5 };
    // ROT = (ROTPins) {
    //    .RBIEINTE = { (unsigned char*) 0x00B, 2, 3 },
    // .RBIE   = { (unsigned char*) 0x00B, 1, 3 },
    // .INTE   = { (unsigned char*) 0x00B, 1, 4 },
    // .GIEPEIE = { (unsigned char*) 0x00B, 2, 6 },
    // .PEIE   = { (unsigned char*) 0x00B, 1, 6 },
    // .GIE    = { (unsigned char*) 0x00B, 1, 7 },
    //    .IOCBA  = { (unsigned char*) 0x096, 1, pinA }, // 0x0D
    //    .IOCBB  = { (unsigned char*) 0x096, 1, pinB },
    //    .INTEDG = { (unsigned char*) 0x081, 1, 6 },
    // .PORTB  = { (unsigned char*) 0x106, 8, 0 },
    // .INTF   = { (unsigned char*) 0x00B, 1, 1 },
    // .RBIF   = { (unsigned char*) 0x00B, 1, 0 }
  // };

  // SetReg(&ROT.INTF, 0);
  //  SetReg(&ROT.RBIF, 0);

   //    SetReg(&ROT.GIEPEIE, 0b11);
   //    SetReg(&ROT.RBIEINTE, 0b11);
   // SetReg(&ROT.GIE, 1);
   // SetReg(&ROT.INTE, 1);
   // SetReg(&ROT.PEIE, 1);
   // SetReg(&ROT.RBIE, 1);
   //    SetReg(&ROT.IOCBA, 1);
   //    SetReg(&ROT.IOCBB, 1);
   //    SetReg(&ROT.INTEDG, 0);

}

void _rot_init_1829() {
  SetRawReg((unsigned char *)0x18C, 1, 0, 0); // ANSELA.0
  SetRawReg((unsigned char *)0x18C, 1, 1, 0); // ANSELA.1

  SetRawReg((unsigned char *)0x08C, 1, 0, 1); // TRISA.0
  SetRawReg((unsigned char *)0x08C, 1, 1, 1); // TRISA.1

  SetRawReg((unsigned char *)0x391, 1, 0, 1); // IOCAP.0
  SetRawReg((unsigned char *)0x391, 1, 1, 1); // IOCAP.1

  SetRawReg((unsigned char *)0x00B, 1, 3, 1); // IOCIE
  SetRawReg((unsigned char *)0x00B, 1, 4, 1); // INTE
  SetRawReg((unsigned char *)0x00B, 1, 6, 1); // PEIE
  SetRawReg((unsigned char *)0x00B, 1, 7, 1); // GIE

  encoder.pinA = (Reg) { (unsigned char*) 0x00C, 1, 0 };
  encoder.pinB = (Reg) { (unsigned char*) 0x00C, 1, 1 };
}

int read_rotary(void) {
  // Only check for a change in the CLK pin to identify "full click"
  int a = GetReg(&encoder.pinA);

  // If state changed, a movement is registered
  if (a != encoder.LastState) {
    int b = GetReg(&encoder.pinB);

    if (b != a) encoder.Steps++; // Clockwise
    else encoder.Steps--; // Anti-clockwise

    // Only change counter if correct amount of clicks have been registered
    if (encoder.Steps > (encoder.stepReq * 2)) {
      encoder.Input++;
      encoder.Steps = 0;
    } else if (encoder.Steps < (-(encoder.stepReq * 2))) {
      encoder.Input--;
      encoder.Steps = 0;
    }

    // Adjust counter for boundaries
    if (encoder.Input < encoder.min) encoder.Input = encoder.max;
    else if (encoder.Input > encoder.max) encoder.Input = encoder.min;

    encoder.LastState = a;
  }

  if (encoder.host == PIC16F887A) SetReg(&encoder.encoderRail, GetReg(&encoder.encoderRail));

  return encoder.Input;
  // PORTC = (unsigned char)(~(1 << (encoder.Input - 1)));
}