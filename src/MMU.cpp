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

MMU::MMU() {
  clearMemory();
  memory[0xFF00] = 0xCF;
  memory[0xFF01] = 0x00;
  memory[0xFF02] = 0x7E;
  memory[0xFF04] = 0x00;
  memory[0xFF05] = 0x00;
  memory[0xFF06] = 0x00;
  memory[0xFF07] = 0x00;
  memory[0xFF0F] = 0xE1;
  memory[0xFF10] = 0x80;
  memory[0xFF11] = 0xBF;
  memory[0xFF12] = 0xF3;
  memory[0xFF13] = 0xFF;
  memory[0xFF14] = 0xBF;
  memory[0xFF16] = 0x3F;
  memory[0xFF17] = 0x00;
  memory[0xFF19] = 0xBF;
  memory[0xFF1A] = 0x7F;
  memory[0xFF1B] = 0xFF;
  memory[0xFF1C] = 0x9F;
  memory[0xFF1D] = 0xFF;
  memory[0xFF1E] = 0xBF;
  memory[0xFF20] = 0xFF;
  memory[0xFF21] = 0x00;
  memory[0xFF22] = 0x00;
  memory[0xFF23] = 0xBF;
  memory[0xFF24] = 0x77;
  memory[0xFF25] = 0xF3;
  memory[0xFF26] = 0xF1;
  memory[0xFF40] = 0x91;
  memory[0xFF41] = 0x85;
  memory[0xFF42] = 0x00;
  memory[0xFF43] = 0x00;
  memory[0xFF44] = 0x00;
  memory[0xFF45] = 0x00;
  memory[0xFF46] = 0xFF;
  memory[0xFF47] = 0xFC;
  memory[0xFF48] = 0xFF;
  memory[0xFF49] = 0xFF;
  memory[0xFF4A] = 0x00;
  memory[0xFF4B] = 0x00;
  memory[0xFF4D] = 0xFF;
  memory[0xFF4F] = 0xFF;
  memory[0xFF51] = 0xFF;
  memory[0xFF52] = 0xFF;
  memory[0xFF53] = 0xFF;
  memory[0xFF54] = 0xFF;
  memory[0xFF55] = 0xFF;
  memory[0xFF56] = 0xFF;
  memory[0xFF68] = 0xFF;
  memory[0xFF69] = 0xFF;
  memory[0xFF6A] = 0xFF;
  memory[0xFF6B] = 0xFF;
  memory[0xFF70] = 0xFF;
  memory[0xFFFF] = 0x00;
}

void MMU::write(uint16_t address, uint8_t data) { memory[address] = data; }

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

  for (size_t i = 0; i < read_only_memory.size(); i++) {
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
