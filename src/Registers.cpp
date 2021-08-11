#include "GB/Registers.hpp"

namespace GB {
Registers::Registers() {}

Registers::~Registers() {}

uint8_t Registers::getA() { return A; }
void Registers::setA(uint8_t data) { A = data; }

uint8_t Registers::getB() { return B; }
void Registers::setB(uint8_t data) { B = data; }

uint8_t Registers::getC() { return C; }
void Registers::setC(uint8_t data) { C = data; }

uint8_t Registers::getD() { return D; }
void Registers::setD(uint8_t data) { D = data; }

uint8_t Registers::getE() { return E; }
void Registers::setE(uint8_t data) { E = data; }

uint8_t Registers::getF() { return F; }
void Registers::setF(uint8_t data) { F = data; }

uint8_t Registers::getH() { return H; }
void Registers::setH(uint8_t data) { H = data; }

uint8_t Registers::getL() { return L; }
void Registers::setL(uint8_t data) { L = data; }

uint16_t Registers::getAF() { return create16BitRegister(A, F); }
void Registers::setAF(uint16_t data) { write16BitData(data, &A, &F); }

uint16_t Registers::getBC() { return create16BitRegister(B, C); }
void Registers::setBC(uint16_t data) { write16BitData(data, &B, &C); }

uint16_t Registers::getDE() { return create16BitRegister(D, E); }
void Registers::setDE(uint16_t data) { write16BitData(data, &D, &E); }

uint16_t Registers::getHL() { return create16BitRegister(H, L); }
void Registers::setHL(uint16_t data) { write16BitData(data, &H, &L); }

uint8_t Registers::getZeroFlag() { return getBit(F, 7); }
void Registers::setZeroFlag() { setBit(&F, 7); }
void Registers::clearZeroFlag() { clearBit(&F, 7); }

uint8_t Registers::getSubtractionFlag() { return getBit(F, 6); }
void Registers::setSubtractionFlag() { setBit(&F, 6); }
void Registers::clearSubtractionFlag() { clearBit(&F, 6); }

uint8_t Registers::getHalfCarryFlag() { return getBit(F, 5); }
void Registers::setHalfCarryFlag() { setBit(&F, 5); }
void Registers::clearHalfCarryFlag() { clearBit(&F, 5); }

uint8_t Registers::getCarryFlag() { return getBit(F, 4); }
void Registers::setCarryFlag() { setBit(&F, 4); }
void Registers::clearCarryFlag() { clearBit(&F, 4); }

uint16_t Registers::create16BitRegister(uint8_t high, uint8_t low) {
  uint16_t high_byte = (high << 8) & 0xff00;
  uint16_t low_byte = low & 0x00ff;
  return high_byte | low_byte;
}

void Registers::write16BitData(uint16_t data, uint8_t *high, uint8_t *low) {
  uint8_t high_data = data >> 8;
  uint8_t low_data = data & 0x00ff;
  *high = high_data;
  *low = low_data;
}

uint8_t Registers::getBit(uint8_t reg, uint8_t position) {
  return (reg >> position) & 0x1;
}

void Registers::setBit(uint8_t *reg, uint8_t position) {
  *reg = *reg | (1 << position);
}
void Registers::clearBit(uint8_t *reg, uint8_t position) {
  *reg = *reg & ~(1 << position);
}
}  // namespace GB
