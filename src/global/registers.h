#ifndef REGISTERS_H
#define REGISTERS_H

typedef enum _picId {
  PIC16F1829,
  PIC16F887A
} picId;

typedef struct _Reg {
  unsigned char* address;
  unsigned char size;
  unsigned char offset;
} Reg;

void SetReg(Reg* R, unsigned char V);
unsigned char SetRawReg(unsigned char* address, unsigned char size, unsigned char offset, unsigned char V);
unsigned char GetReg(Reg* R);

#endif