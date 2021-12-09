#include "GB/MMU.hpp"

namespace GB
{
void MMU::setWindowXRegister(uint8_t data)
{
    memory.at(WX_addr) = data;
}
uint8_t MMU::getWindowXRegister()
{
    return memory.at(WX_addr);
}
void MMU::setWindowYRegister(uint8_t data)
{
    memory.at(WY_addr) = data;
}
uint8_t MMU::getWindowYRegister()
{
    return memory.at(WY_addr);
}
uint8_t MMU::getScrollXRegister()
{
    return memory.at(SCX_addr);
}
uint8_t MMU::getScrollYRegister()
{
    return memory.at(SCY_addr);
}

void MMU::setLCDDisplayEnable()
{
    memory.at(LCDC_addr) = setBit(7, memory.at(LCDC_addr));
}
void MMU::resetLCDDisplayEnable()
{
    memory.at(LCDC_addr) = resetBit(7, memory.at(LCDC_addr));
}
bool MMU::isLCDDisplayEnabled()
{
    return checkBit(7, memory.at(LCDC_addr));
}

void MMU::setWindowTileMapSelect()
{
    memory.at(LCDC_addr) = setBit(6, memory.at(LCDC_addr));
}
void MMU::resetWindowTileMapSelect()
{
    memory.at(LCDC_addr) = resetBit(6, memory.at(LCDC_addr));
}
bool MMU::isWindowTileMapSelectSet()
{
    return checkBit(6, memory.at(LCDC_addr));
}

void MMU::setWindowDisplayEnable()
{
    memory.at(LCDC_addr) = setBit(5, memory.at(LCDC_addr));
}
void MMU::resetWindowDisplayEnable()
{
    memory.at(LCDC_addr) = resetBit(5, memory.at(LCDC_addr));
}
bool MMU::isWindowDisplayEnabled()
{
    return checkBit(5, memory.at(LCDC_addr));
}

void MMU::setTileDataSelect()
{
    memory.at(LCDC_addr) = setBit(4, memory.at(LCDC_addr));
}
void MMU::resetTileDataSelect()
{
    memory.at(LCDC_addr) = resetBit(4, memory.at(LCDC_addr));
}
bool MMU::isTileDataSelectSet()
{
    return checkBit(4, memory.at(LCDC_addr));
}

void MMU::setBGTileMapSelect()
{
    memory.at(LCDC_addr) = setBit(3, memory.at(LCDC_addr));
}
void MMU::resetBGTileMapSelect()
{
    memory.at(LCDC_addr) = resetBit(3, memory.at(LCDC_addr));
}
bool MMU::isBGTileMapSelectSet()
{
    return checkBit(3, memory.at(LCDC_addr));
}

void MMU::setTallSpriteSize()
{
    memory.at(LCDC_addr) = setBit(2, memory.at(LCDC_addr));
}
void MMU::resetTallSpriteSize()
{
    memory.at(LCDC_addr) = resetBit(2, memory.at(LCDC_addr));
}
bool MMU::isTallSpriteSizeSet()
{
    return checkBit(2, memory.at(LCDC_addr));
}

void MMU::setSpriteEnable()
{
    memory.at(LCDC_addr) = setBit(1, memory.at(LCDC_addr));
}
void MMU::resetSpriteEnable()
{
    memory.at(LCDC_addr) = resetBit(1, memory.at(LCDC_addr));
}
bool MMU::isSpritesEnabled()
{
    return checkBit(1, memory.at(LCDC_addr));
}

void MMU::setBGAndWindowEnable()
{
    memory.at(LCDC_addr) = setBit(0, memory.at(LCDC_addr));
}
void MMU::resetBGAndWindowEnable()
{
    memory.at(LCDC_addr) = resetBit(0, memory.at(LCDC_addr));
}
bool MMU::isBGAndWindowEnabled()
{
    return checkBit(0, memory.at(LCDC_addr));
}

void MMU::setLYCEqualLYSTATInterruptEnable()
{
    memory.at(STAT_addr) = setBit(6, memory.at(STAT_addr));
}
void MMU::resetLYCEqualLYSTATInterruptEnable()
{
    memory.at(STAT_addr) = resetBit(6, memory.at(STAT_addr));
}
bool MMU::isLYCEqualLYSTATInterruptEnabled()
{
    return checkBit(6, memory.at(STAT_addr));
}

void MMU::setMode2STATInterruptEnable()
{
    memory.at(STAT_addr) = setBit(5, memory.at(STAT_addr));
}
void MMU::resetMode2STATInterruptEnable()
{
    memory.at(STAT_addr) = resetBit(5, memory.at(STAT_addr));
}
bool MMU::isMode2STATInterruptEnabled()
{
    return checkBit(5, memory.at(STAT_addr));
}

void MMU::setMode1STATInterruptEnable()
{
    memory.at(STAT_addr) = setBit(4, memory.at(STAT_addr));
}
void MMU::resetMode1STATInterruptEnable()
{
    memory.at(STAT_addr) = resetBit(4, memory.at(STAT_addr));
}
bool MMU::isMode1STATInterruptEnabled()
{
    return checkBit(4, memory.at(STAT_addr));
}

void MMU::setMode0STATInterruptEnable()
{
    memory.at(STAT_addr) = setBit(3, memory.at(STAT_addr));
}
void MMU::resetMode0STATInterruptEnable()
{
    memory.at(STAT_addr) = resetBit(3, memory.at(STAT_addr));
}
bool MMU::isMode0STATInterruptEnabled()
{
    return checkBit(3, memory.at(STAT_addr));
}

void MMU::setCoincidenceFlag()
{
    memory.at(STAT_addr) = setBit(2, memory.at(STAT_addr));
}
void MMU::resetCoincidenceFlag()
{
    memory.at(STAT_addr) = resetBit(2, memory.at(STAT_addr));
}
bool MMU::isCoincidenceFlagSet()
{
    return checkBit(2, memory.at(STAT_addr));
}
void MMU::setPPUMode(uint8_t mode)
{
    memory.at(STAT_addr) = (memory.at(STAT_addr) & 0xFC) | mode;
}
uint8_t MMU::getPPUMode()
{
    return memory.at(STAT_addr) & 0x03;
}

uint16_t MMU::getPPU8000MethodAddress(uint8_t tile_number)
{
    return tile_number * 16 + 0x8000;
}
uint16_t MMU::getPPU8800MethodAddress(uint8_t tile_number)
{
    return static_cast<int8_t>(tile_number) * 16 + 0x9000;
}

void MMU::setCurrentScanLine(uint8_t data)
{
    memory.at(LY_addr) = data;
}
uint8_t MMU::getCurrentScanLine()
{
    return memory.at(LY_addr);
}
void MMU::incrementCurrentScanLine()
{
    memory.at(LY_addr)++;
}

bool MMU::isLYCEqualLY()
{
    return memory.at(LY_addr) == memory.at(LYC_addr);
}

uint32_t MMU::getOAMSpriteEntry(uint16_t offset)
{
    uint16_t index = 4 * offset;
    uint8_t y = memory.at(index + OAM_addr);
    uint8_t x = memory.at(index + OAM_addr + 1);
    uint8_t tile = memory.at(index + OAM_addr + 2);
    uint8_t flags = memory.at(index + OAM_addr + 3);
    return (flags << 24) | (tile << 16) | (x << 8) | y;
}

} // namespace GB
