#ifndef _IRLib_V7_H_
#define _IRLib_V7_H_

typedef struct _Reg {
    volatile unsigned char *address;
    unsigned char size;
    unsigned char offset;
} Reg;

typedef enum {
    falling,
    rising
} IntEdge;

#define MessageSize 12

volatile int dataBuffer = 0; //data buffer
volatile char IRBF = 0; //infrared buffer full flag
volatile int data = 0;
volatile char count = 0;

void IRInit(IntEdge, unsigned char, unsigned char, unsigned char, unsigned char); //edge, MTPR, MTPrSc, MTPoSc, TTPrSc
void IntHandler(void);
void MTHandler(void);
void TTHandler(void);
void SetReg(Reg*, unsigned char);
unsigned char ReadReg(Reg*);

Reg IRR; // Infra red reciever / INT Pin
Reg MTimer; // Measure Timer
Reg MTOn; // MTimer on
Reg MTIF; // MTimer interrupt Flag
Reg MTPR; // MTimer Period Register
Reg MTPrSc; // MTimer Pre-Scaler
Reg MTPoSc; // MTimer Post-Scaler
Reg TTimer; // Timeout Timer
Reg TTOn; // TTimer on
Reg TTIF; // TTimer Interrupt Flag
Reg TTPrSc; // TTimer Pre-Scaler
Reg IntPinTrisReg; // Interrupt Pin TriState Register
Reg IntPinAnselReg; // Interrupt Pin Analog Select Register
Reg IntPinIF; // Interrupt Pin Interrupt Flag
Reg IntEdgeReg; // Interrupt pin Edge Select Register
Reg IntConGIE; // Global Interrupt Enable
Reg IntConINTE; // Interrupt Enable
Reg IntConPEIE; // Peripheral Interrupt Enable
Reg PIEMTIE; // Peripheral Interrupt Enable Measure Timer Interrupt Enable
Reg PIETTIE; // Peripheral Interrupt Enable Timeout Timer Interrupt Enable

void IRBind887(void);
void IRBind1829(void);

#endif //_IRLIB_V7_H_