#include "GB/MMU.hpp"

#include <fstream>
#include <iostream>

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

void MMU::write(uint16_t address, uint8_t data) {
  if (address >= 0x0000 && address <= 0x7FFF) {
    handleBankedROMWrite(address, data);
  } else if (address >= 0xA000 && address <= 0xBFFF) {
    handleBankedRAMWrite(address, data);
  } else if (address >= 0xE000 && address <= 0xFDFF) {
    handleEchoWrite(address, data);
  } else if (address >= 0xFEA0 && address <= 0xFEFF) {
    return;
  } else if (address == DIV_addr) {
    memory[DIV_addr] = 0x00;
  } else if (address == LY_addr) {
    memory[LY_addr] = 0x00;
  } else if (address == DMA_addr) {
    doDMATransferToOAM(data);
  } else {
    memory[address] = data;
  }
}

void MMU::handleEchoWrite(uint16_t address, uint8_t data) {
  constexpr uint16_t offset{0xE000 - 0xC000};
  memory[address] = data;
  memory[address - offset] = data;
}

void MMU::doDMATransferToOAM(uint8_t data) {
  uint16_t address = data << 8;
  const int dma_byte_size = 160;
  for (int i = 0; i < dma_byte_size; ++i) {
    write(0xFE00 + i, read(address + i));
  }
}

uint8_t MMU::read(uint16_t address) {
  if (address >= 0x0000 && address <= 0x7FFF) {
    return handleBankedROMRead(address);
  } else if (address >= 0xA000 && address <= 0xBFFF) {
    return handleBankedRAMRead(address);
  } else if (address >= 0xE000 && address <= 0xFDFF) {
    return handleEchoRead(address);
  } else if (address == JOYP_addr) {
    return getJOYPRegister();
  } else {
    return memory[address];
  }
}

uint8_t MMU::handleEchoRead(uint16_t address) {
  constexpr uint16_t offset{0xE000 - 0xC000};
  return memory[address - offset];
}

void MMU::clearMemory() {
  for (size_t i = 0; i < memory.size(); i++) {
    memory[i] = 0x00;
  }
}
void MMU::loadROM(const char* rom_path) {
  // TODO: optimize memory management better due to banking
  std::ifstream rom_file(rom_path, std::ios::binary);
  if (!rom_file.is_open()) {
    throw std::runtime_error("Cannot read ROM: " + std::string(rom_path));
  }
  while (rom_file) {
    read_only_memory.push_back(rom_file.get());
  }

  updateMemoryBankController();
  updateROMSize();
  updateRAMSize();
  std::cout << (int)memory_bank_controller << "\n";

  // TODO: don't need this anymore probably
  // But I don't want to refactor my writing / reading code extensively
  for (size_t i = 0; i < 0x8000; i++) {
    memory[i] = read_only_memory[i];
  }

  transferROMToBanks();
}

void MMU::updateMemoryBankController() {
  constexpr uint16_t controller_type_addr{0x0147};
  switch (read_only_memory[controller_type_addr]) {
    case 0x00:
      memory_bank_controller = 0x00;
      break;
    case 0x01:
      memory_bank_controller = 0x01;
      break;
    case 0x02:
      memory_bank_controller = 0x01;
      break;
    case 0x03:
      memory_bank_controller = 0x01;
      break;
    default:
      throw std::runtime_error("Unsupported banking");
  }
}

void MMU::updateROMSize() {
  constexpr uint16_t ROM_size_addr{0x0148};
  uint8_t code = read_only_memory[ROM_size_addr];
  if (code > 0x08) {
    throw std::runtime_error("Unsupported ROM size");
  }
  size_t num_banks = 2 << code;
  rom_banks.resize(num_banks);
}

void MMU::updateRAMSize() {
  constexpr uint16_t RAM_size_addr{0x0149};
  uint8_t code = read_only_memory[RAM_size_addr];
  switch (code) {
    case 0x00:
      return;
      break;
    case 0x01:
      return;
      break;
    case 0x02:
      ram_banks.resize(1);
      break;
    case 0x03:
      ram_banks.resize(4);
      break;
    case 0x04:
      ram_banks.resize(16);
      break;
    case 0x05:
      ram_banks.resize(8);
      break;
    default:
      throw std::runtime_error("Unsupported RAM size");
  }
}

void MMU::transferROMToBanks() {
  for (size_t bank_no = 0, rom_idx = 0;
       bank_no < rom_banks.size() && rom_idx < read_only_memory.size();
       bank_no++, rom_idx += 0x4000) {
    for (size_t i = 0; i < 0x4000; ++i) {
      rom_banks[bank_no][i] = read_only_memory[rom_idx + i];
    }
  }
}

}  // namespace GB
