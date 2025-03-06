#include "registers.h"

void SetReg(Reg* R, unsigned char V) {
  unsigned char bitMask;
  V = (unsigned char) (V << R->offset);
  bitMask = (unsigned char) (((1 << R->size) - 1) << R->offset);
  *(R->address) &= ~bitMask;
  *(R->address) |= V & bitMask;
}

unsigned char GetReg(Reg* R) {
    unsigned char V, bitMask;
    bitMask = (unsigned char) (((1 << R->size) - 1) << R->offset);
    V = *(R->address) & bitMask;
    V = V >> R->offset;
    return V;
}

void SetRawReg(unsigned char* address, unsigned char size, unsigned char offset, unsigned char V) {
  unsigned char bitMask;
  V = (unsigned char) (V << offset);
  bitMask = (unsigned char) (((1 << size) - 1) << offset);
  *(address) &= ~bitMask;
  *(address) |= V & bitMask;
}