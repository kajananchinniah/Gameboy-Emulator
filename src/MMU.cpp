#include "GB/MMU.hpp"

#include <fstream>
#include <iostream>

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
  uint8_t enable_bit = (TAC >> 2) & 0x1;
  if (enable_bit == 1) {
    return true;
  } else {
    return false;
  }
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
  if ((memory[TIMA_addr] + 1) >= 256) {
    return true;
  } else {
    return false;
  }
}

void MMU::setTimerInterrupt() { memory[IF_addr] = memory[IF_addr] | (1 << 2); }

}  // namespace GB
