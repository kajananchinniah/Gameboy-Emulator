#include "GB/MMU.hpp"

namespace GB
{

void MMU::handleBankedROMWrite_MBC1(uint16_t address, uint8_t data)
{
    if (address >= 0x0000 && address <= 0x1FFF) {
        uint8_t ram_enable_code = data & 0x0F;
        if (ram_enable_code == 0x0A) {
            ram_enabled = true;
        } else {
            ram_enabled = false;
        }
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        current_rom_bank = data & 0x1F;
        if (current_rom_bank == 0x00 || current_rom_bank == 0x20 ||
            current_rom_bank == 0x40 || current_rom_bank == 0x60) {
            current_rom_bank++;
        }
    } else if (address >= 0x4000 && address <= 0x5FFF) {
        if (banking_mode == 0x00) {
            current_rom_bank = ((data & 0x03) << 5) | (current_rom_bank & 0x1F);
            if (current_rom_bank == 0x00 || current_rom_bank == 0x20 ||
                current_rom_bank == 0x40 || current_rom_bank == 0x60) {
                current_rom_bank++;
            }
        } else {
            current_ram_bank = data & 0x03;
        }

    } else if (address >= 0x6000 && address <= 0x7FFF) {
        banking_mode = data & 0x01;
        // ROM mode
        if (banking_mode == 0x00) {
            current_ram_bank = 0x00;
        }
    }
}

void MMU::handleBankedRAMWrite_MBC1(uint16_t address, uint8_t data)
{
    if (ram_enabled) {
        uint16_t effective_address = address - 0xA000;
        ram_banks.at(current_ram_bank).at(effective_address) = data;
    }
}

uint8_t MMU::handleBankedROMRead_MBC1(uint16_t address)
{
    if (address >= 0x0000 && address <= 0x3FFF) {
        return rom_banks.at(0).at(address);
    } else if (address >= 0x4000 && address <= 0x7FFF) {
        uint16_t effective_address = address - 0x4000;
        return rom_banks.at(current_rom_bank).at(effective_address);
    } else {
        return memory.at(address);
    }
}

uint8_t MMU::handleBankedRAMRead_MBC1(uint16_t address)
{
    uint16_t effective_address = address - 0xA000;
    return ram_banks.at(current_ram_bank).at(effective_address);
}

} // namespace GB
