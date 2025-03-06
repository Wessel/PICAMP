#ifndef _IRLib_V7_H_
#define _IRLib_V7_H_

typedef struct _IR_Reg {
    volatile unsigned char *address;
    unsigned char size;
    unsigned char offset;
} IR_Reg;

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
void IR_SetReg(IR_Reg*, unsigned char);
unsigned char ReadReg(IR_Reg*);

IR_Reg IRR; // Infra red reciever / INT Pin
IR_Reg MTimer; // Measure Timer
IR_Reg MTOn; // MTimer on
IR_Reg MTIF; // MTimer interrupt Flag
IR_Reg MTPR; // MTimer Period Register
IR_Reg MTPrSc; // MTimer Pre-Scaler
IR_Reg MTPoSc; // MTimer Post-Scaler
IR_Reg TTimer; // Timeout Timer
IR_Reg TTOn; // TTimer on
IR_Reg TTIF; // TTimer Interrupt Flag
IR_Reg TTPrSc; // TTimer Pre-Scaler
IR_Reg IntPinTrisReg; // Interrupt Pin TriState Register
IR_Reg IntPinAnselReg; // Interrupt Pin Analog Select Register
IR_Reg IntPinIF; // Interrupt Pin Interrupt Flag
IR_Reg IntEdgeReg; // Interrupt pin Edge Select Register
IR_Reg IntConGIE; // Global Interrupt Enable
IR_Reg IntConINTE; // Interrupt Enable
IR_Reg IntConPEIE; // Peripheral Interrupt Enable
IR_Reg PIEMTIE; // Peripheral Interrupt Enable Measure Timer Interrupt Enable
IR_Reg PIETTIE; // Peripheral Interrupt Enable Timeout Timer Interrupt Enable

void IRBind887(void);
void IRBind1829(void);

#endif //_IRLIB_V7_H_