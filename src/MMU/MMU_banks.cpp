#include <stdexcept>

#include "GB/MMU.hpp"

namespace GB {
void MMU::handleBankedROMWrite(uint16_t address, uint8_t data) {
  switch (memory_bank_controller) {
    case 0x00:
      handleBankedROMWrite_MBC0(address, data);
      break;
    case 0x01:
      handleBankedROMWrite_MBC1(address, data);
      break;
  }
}

void MMU::handleBankedRAMWrite(uint16_t address, uint8_t data) {
  switch (memory_bank_controller) {
    case 0x00:
      handleBankedRAMWrite_MBC0(address, data);
      break;
    case 0x01:
      handleBankedRAMWrite_MBC1(address, data);
      break;
  }
}

uint8_t MMU::handleBankedROMRead(uint16_t address) {
  switch (memory_bank_controller) {
    case 0x00:
      return handleBankedROMRead_MBC0(address);
    case 0x01:
      return handleBankedROMRead_MBC1(address);
    default:
      throw std::runtime_error("Unsupported banking mode");
  }
}

uint8_t MMU::handleBankedRAMRead(uint16_t address) {
  switch (memory_bank_controller) {
    case 0x00:
      return handleBankedRAMRead_MBC0(address);
    case 0x01:
      return handleBankedRAMRead_MBC1(address);
    default:
      throw std::runtime_error("Unsupported banking mode");
  }
}

}  // namespace GB
