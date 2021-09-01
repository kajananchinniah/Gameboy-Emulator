#include "GB/Registers.hpp"

namespace GB {

uint16_t *Register::getFullRegister() { return &data_.full; }
uint8_t *Register::getHighRegister() { return &data_.high; }
uint8_t *Register::getLowRegister() { return &data_.low; }

uint16_t Register::getFullValue() { return data_.full; }
uint8_t Register::getHighValue() { return data_.high; }
uint8_t Register::getLowValue() { return data_.low; }

void Register::setFullValue(uint16_t data) { data_.full = data; }
void Register::setHighValue(uint8_t data) { data_.high = data; }
void Register::setLowValue(uint8_t data) { data_.low = data; }

uint8_t RegisterAF::getZeroFlag() { return getBit(data_.low, 7); }
void RegisterAF::setZeroFlag() { setBit(&data_.low, 7); }
void RegisterAF::clearZeroFlag() { clearBit(&data_.low, 7); }

uint8_t RegisterAF::getSubtractionFlag() { return getBit(data_.low, 6); }
void RegisterAF::setSubtractionFlag() { setBit(&data_.low, 6); }
void RegisterAF::clearSubtractionFlag() { clearBit(&data_.low, 6); }

uint8_t RegisterAF::getHalfCarryFlag() { return getBit(data_.low, 5); }
void RegisterAF::setHalfCarryFlag() { setBit(&data_.low, 5); }
void RegisterAF::clearHalfCarryFlag() { clearBit(&data_.low, 5); }

uint8_t RegisterAF::getCarryFlag() { return getBit(data_.low, 4); }
void RegisterAF::setCarryFlag() { setBit(&data_.low, 4); }
void RegisterAF::clearCarryFlag() { clearBit(&data_.low, 4); }
void RegisterAF::xorCarryFlag() {
  uint8_t carry = getCarryFlag();
  carry = carry ^ 1;
  if (carry == 1) {
    setCarryFlag();
  } else {
    clearCarryFlag();
  }
}

uint8_t RegisterAF::getBit(uint8_t reg, uint8_t position) {
  return (reg >> position) & 0x1;
}

void RegisterAF::setBit(uint8_t *reg, uint8_t position) {
  *reg = *reg | (1 << position);
}
void RegisterAF::clearBit(uint8_t *reg, uint8_t position) {
  *reg = *reg & ~(1 << position);
}

void ProgramCounter::incrementPC(uint16_t amount) { PC += amount; }
void ProgramCounter::decrementPC(uint16_t amount) { PC -= amount; }
void ProgramCounter::setPC(uint16_t data) { PC = data; }
uint16_t ProgramCounter::getPCValue() { return PC; }
uint16_t *ProgramCounter::getPC() { return &PC; }

void StackPointer::incrementSP(uint16_t amount) { SP += amount; }
void StackPointer::decrementSP(uint16_t amount) { SP -= amount; }
void StackPointer::setSP(uint16_t data) { SP = data; }
uint16_t StackPointer::getSPValue() { return SP; }
uint16_t *StackPointer::getSP() { return &SP; }
}  // namespace GB
