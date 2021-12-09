#include "GB/MMU.hpp"

namespace GB
{

void MMU::handleBankedROMWrite_MBC0(uint16_t address, uint8_t data)
{
    if (address >= 0x8000) {
        memory.at(address) = data;
    }
}

void MMU::handleBankedRAMWrite_MBC0(uint16_t address, uint8_t data)
{
    if (address >= 0x8000) {
        memory.at(address) = data;
    }
}

uint8_t MMU::handleBankedROMRead_MBC0(uint16_t address)
{
    return memory.at(address);
}

uint8_t MMU::handleBankedRAMRead_MBC0(uint16_t address)
{
    return memory.at(address);
}

} // namespace GB
