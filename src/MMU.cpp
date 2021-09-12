#include "GB/MMU.hpp"

#include <fstream>
#include <iostream>

namespace {
bool checkBit(uint8_t n, uint8_t value) {
  if ((value >> n) & 0x01) {
    return true;
  } else {
    return false;
  }
}

uint8_t resetBit(uint8_t n, uint8_t value) { return value & ~(1 << n); }
}  // namespace

namespace GB {

void MMU::write(uint16_t address, uint8_t data) {
  if (address == DIV_addr) {
    memory[address] = 0x00;
  } else {
    memory[address] = data;
  }
}

uint8_t MMU::read(uint16_t address) { return memory[address]; }
void MMU::clearMemory() {
  for (size_t i = 0; i < memory.size(); i++) {
    memory[i] = 0x00;
  }
}
void MMU::loadROM(const char* rom_path) {
  std::ifstream rom_file(rom_path, std::ios::binary);
  if (!rom_file.is_open()) {
    throw std::runtime_error("Cannot read ROM: " + std::string(rom_path));
  }
  while (rom_file) {
    read_only_memory.push_back(rom_file.get());
  }

  for (size_t i = 0; i < memory.size(); i++) {
    memory[i] = read_only_memory[i];
  }
}

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
