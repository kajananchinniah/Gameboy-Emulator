#include "GB/MMU.hpp"

namespace GB {

void MMU::write(uint16_t address, uint8_t data) { memory[address] = data; }
uint8_t MMU::read(uint16_t address) { return memory[address]; }
void MMU::clearMemory() {
  for (auto i = 0; i < memory.size(); i++) {
    memory[i] = 0x00;
  }
}
}  // namespace GB
