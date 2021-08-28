#include "GB/MMU.hpp"

#include <fstream>
#include <iostream>

namespace GB {

void MMU::write(uint16_t address, uint8_t data) { memory[address] = data; }
uint8_t MMU::read(uint16_t address) { return memory[address]; }
void MMU::clearMemory() {
  for (size_t i = 0; i < memory.size(); i++) {
    memory[i] = 0x00;
  }
}
void MMU::loadROM(const char* rom_path) {
  std::ifstream rom_file(rom_path, std::ios::binary);
  std::string tmp;
  if (rom_file.is_open()) {
    while (rom_file) {
      tmp.push_back(rom_file.get());
      read_only_memory.push_back(rom_file.get());
    }
  }

  for (int i = 0; i < 0x8000; ++i) {
    write(i, tmp[i]);
  }
}
}  // namespace GB
