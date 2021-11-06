#include <stdexcept>

#include "GB/MMU.hpp"

namespace GB {
void MMU::handleBankedROMWrite(uint16_t address, uint8_t data) {
  switch (memory_bank_controller) {
    case 0x00:
      handleBankedROMWrite_MBC0(address, data);
      break;
  }
}

void MMU::handleBankedRAMWrite(uint16_t address, uint8_t data) {
  switch (memory_bank_controller) {
    case 0x00:
      handleBankedRAMWrite_MBC0(address, data);
      break;
  }
}

uint8_t MMU::handleBankedROMRead(uint16_t address) {
  switch (memory_bank_controller) {
    case 0x00:
      return handleBankedROMRead_MBC0(address);
    default:
      throw std::runtime_error("Unsupported banking mode");
  }
}

uint8_t MMU::handleBankedRAMRead(uint16_t address) {
  switch (memory_bank_controller) {
    case 0x00:
      return handleBankedRAMRead_MBC0(address);
    default:
      throw std::runtime_error("Unsupported banking mode");
  }
}

}  // namespace GB
