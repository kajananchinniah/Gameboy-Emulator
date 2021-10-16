#include "GB/MMU.hpp"

namespace GB {
void MMU::incrementDividerRegister(uint8_t amount) {
  memory[DIV_addr] += amount;
}

void MMU::setDividerRegister(uint8_t data) { memory[DIV_addr] = data; }

uint8_t MMU::getDividerRegister() { return memory[DIV_addr]; }

bool MMU::isTimerEnabled() {
  uint8_t TAC = memory[TAC_addr];
  return checkBit(2, TAC);
}

uint8_t MMU::getInputClockSelect() {
  uint8_t TAC = memory[TAC_addr];
  return TAC & 0x03;
}

void MMU::incrementTimerCounterRegister(uint8_t amount) {
  memory[TIMA_addr] += amount;
}

void MMU::resetTimerCounterRegister() { memory[TIMA_addr] = memory[TMA_addr]; }

uint8_t MMU::getTimerCounterRegister() { return memory[TIMA_addr]; }

bool MMU::willTimerCounterRegisterOverflow() {
  // 8 bit value so 255 is the max it can hold
  if (memory[TIMA_addr] == 255) {
    return true;
  } else {
    return false;
  }
}

void MMU::setTimerInterrupt() { memory[IF_addr] = memory[IF_addr] | (1 << 2); }

bool MMU::isAnyInterruptEnabled() {
  uint8_t interrupt_bit = memory[IE_addr];
  return (interrupt_bit & 0x1F) > 0;
}

bool MMU::isAnyInterruptRequested() {
  uint8_t interrupt_bit = memory[IF_addr];
  return (interrupt_bit & 0x1F) > 0;
}

bool MMU::isVBlankInterruptEnabled() {
  uint8_t interrupt_bit = memory[IE_addr];
  return checkBit(0, interrupt_bit);
}

bool MMU::isVBlankInterruptRequested() {
  uint8_t interrupt_bit = memory[IF_addr];
  return checkBit(0, interrupt_bit);
}

bool MMU::isLCDStatInterruptEnabled() {
  uint8_t interrupt_bit = memory[IE_addr];
  return checkBit(1, interrupt_bit);
}

bool MMU::isLCDStatInterruptRequested() {
  uint8_t interrupt_bit = memory[IF_addr];
  return checkBit(1, interrupt_bit);
}

bool MMU::isTimerInterruptEnabled() {
  uint8_t interrupt_bit = memory[IE_addr];
  return checkBit(2, interrupt_bit);
}

bool MMU::isTimerInterruptRequested() {
  uint8_t interrupt_bit = memory[IF_addr];
  return checkBit(2, interrupt_bit);
}

bool MMU::isSerialInterruptEnabled() {
  uint8_t interrupt_bit = memory[IE_addr];
  return checkBit(3, interrupt_bit);
}

bool MMU::isSerialInterruptRequested() {
  uint8_t interrupt_bit = memory[IF_addr];
  return checkBit(3, interrupt_bit);
}

bool MMU::isJoypadInterruptEnabled() {
  uint8_t interrupt_bit = memory[IE_addr];
  return checkBit(4, interrupt_bit);
}

bool MMU::isJoypadInterruptRequested() {
  uint8_t interrupt_bit = memory[IF_addr];
  return checkBit(4, interrupt_bit);
}

void MMU::setVBlankInterruptRequest() {
  memory[IF_addr] = setBit(0, memory[IF_addr]);
}

void MMU::setLCDStatInterruptRequest() {
  memory[IF_addr] = setBit(1, memory[IF_addr]);
}

void MMU::setTimerInterruptRequest() {
  memory[IF_addr] = setBit(2, memory[IF_addr]);
}

void MMU::setSerialInterruptRequest() {
  memory[IF_addr] = setBit(3, memory[IF_addr]);
}

void MMU::setJoypadInterruptRequest() {
  memory[IF_addr] = setBit(4, memory[IF_addr]);
}

void MMU::resetVBlankInterruptRequest() {
  memory[IF_addr] = resetBit(0, memory[IF_addr]);
}

void MMU::resetLCDStatInterruptRequest() {
  memory[IF_addr] = resetBit(1, memory[IF_addr]);
}

void MMU::resetTimerInterruptRequest() {
  memory[IF_addr] = resetBit(2, memory[IF_addr]);
}

void MMU::resetSerialInterruptRequest() {
  memory[IF_addr] = resetBit(3, memory[IF_addr]);
}

void MMU::resetJoypadInterruptRequest() {
  memory[IF_addr] = resetBit(4, memory[IF_addr]);
}
}  // namespace GB
