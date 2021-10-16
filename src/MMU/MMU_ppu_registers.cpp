#include "GB/MMU.hpp"

namespace GB {
void MMU::setWindowXRegister(uint8_t data) { memory[WX_addr] = data; }
uint8_t MMU::getWindowXRegister() { return memory[WX_addr]; }
void MMU::setWindowYRegister(uint8_t data) { memory[WY_addr] = data; }
uint8_t MMU::getWindowYRegister() { return memory[WY_addr]; }

void MMU::setLCDDisplayEnable() {
  memory[LCDC_addr] = setBit(7, memory[LCDC_addr]);
}
void MMU::resetLCDDisplayEnable() {
  memory[LCDC_addr] = resetBit(7, memory[LCDC_addr]);
}
bool MMU::isLCDDisplayEnabled() { return checkBit(7, memory[LCDC_addr]); }

void MMU::setWindowTileMapSelect() {
  memory[LCDC_addr] = setBit(6, memory[LCDC_addr]);
}
void MMU::resetWindowTileMapSelect() {
  memory[LCDC_addr] = resetBit(6, memory[LCDC_addr]);
}
bool MMU::isWindowTileMapSelectSet() { return checkBit(6, memory[LCDC_addr]); }

void MMU::setWindowDisplayEnable() {
  memory[LCDC_addr] = setBit(5, memory[LCDC_addr]);
}
void MMU::resetWindowDisplayEnable() {
  memory[LCDC_addr] = resetBit(5, memory[LCDC_addr]);
}
bool MMU::isWindowDisplayEnabled() { return checkBit(5, memory[LCDC_addr]); }

void MMU::setTileDataSelect() {
  memory[LCDC_addr] = setBit(4, memory[LCDC_addr]);
}
void MMU::resetTileDataSelect() {
  memory[LCDC_addr] = resetBit(4, memory[LCDC_addr]);
}
bool MMU::isTileDataSelectSet() { return checkBit(4, memory[LCDC_addr]); }

void MMU::setBGTileMapSelect() {
  memory[LCDC_addr] = setBit(3, memory[LCDC_addr]);
}
void MMU::resetBGTileMapSelect() {
  memory[LCDC_addr] = resetBit(3, memory[LCDC_addr]);
}
bool MMU::isBGTileMapSelectSet() { return checkBit(3, memory[LCDC_addr]); }

void MMU::setTallSpriteSize() {
  memory[LCDC_addr] = setBit(2, memory[LCDC_addr]);
}
void MMU::resetTallSpriteSize() {
  memory[LCDC_addr] = resetBit(2, memory[LCDC_addr]);
}
bool MMU::isTallSpriteSizeSet() { return checkBit(2, memory[LCDC_addr]); }

void MMU::setSpriteEnable() {
  memory[LCDC_addr] = setBit(1, memory[LCDC_addr]);
}
void MMU::resetSpriteEnable() {
  memory[LCDC_addr] = resetBit(1, memory[LCDC_addr]);
}
bool MMU::isSpritesEnabled() { return checkBit(1, memory[LCDC_addr]); }

void MMU::setBGAndWindowEnable() {
  memory[LCDC_addr] = setBit(0, memory[LCDC_addr]);
}
void MMU::resetBGAndWindowEnable() {
  memory[LCDC_addr] = resetBit(0, memory[LCDC_addr]);
}
bool MMU::isBGAndWindowEnabled() { return checkBit(0, memory[LCDC_addr]); }

void MMU::setLYCEqualLYSTATInterruptEnable() {
  memory[STAT_addr] = setBit(6, memory[STAT_addr]);
}
void MMU::resetLYCEqualLYSTATInterruptEnable() {
  memory[STAT_addr] = resetBit(6, memory[STAT_addr]);
}
bool MMU::isLYCEqualLYSTATInterruptEnabled() {
  return checkBit(6, memory[STAT_addr]);
}

void MMU::setMode2STATInterruptEnable() {
  memory[STAT_addr] = setBit(5, memory[STAT_addr]);
}
void MMU::resetMode2STATInterruptEnable() {
  memory[STAT_addr] = resetBit(5, memory[STAT_addr]);
}
bool MMU::isMode2STATInterruptEnabled() {
  return checkBit(5, memory[STAT_addr]);
}

void MMU::setMode1STATInterruptEnable() {
  memory[STAT_addr] = setBit(4, memory[STAT_addr]);
}
void MMU::resetMode1STATInterruptEnable() {
  memory[STAT_addr] = resetBit(4, memory[STAT_addr]);
}
bool MMU::isMode1STATInterruptEnabled() {
  return checkBit(4, memory[STAT_addr]);
}

void MMU::setMode0STATInterruptEnable() {
  memory[STAT_addr] = setBit(3, memory[STAT_addr]);
}
void MMU::resetMode0STATInterruptEnable() {
  memory[STAT_addr] = resetBit(3, memory[STAT_addr]);
}
bool MMU::isMode0STATInterruptEnabled() {
  return checkBit(3, memory[STAT_addr]);
}

void MMU::setCoincidenceFlag() {
  memory[STAT_addr] = setBit(2, memory[STAT_addr]);
}
void MMU::resetCoincidenceFlag() {
  memory[STAT_addr] = resetBit(2, memory[STAT_addr]);
}
bool MMU::isCoincidenceFlagSet() { return checkBit(2, memory[STAT_addr]); }
void MMU::setPPUMode(uint8_t mode) {
  memory[STAT_addr] = (memory[STAT_addr] & 0xFC) | mode;
}
uint8_t MMU::getPPUMode() { return memory[STAT_addr] & 0x03; }

uint16_t MMU::getPPU8000MethodAddress(uint8_t tile_number) {
  return tile_number * 16 + 0x8000;
}
uint16_t MMU::getPPU8800MethodAddress(uint8_t tile_number) {
  return static_cast<int8_t>(tile_number) * 16 + 0x9000;
}

void MMU::setCurrentScanLine(uint8_t data) { memory[LY_addr] = data; }
uint8_t MMU::getCurrentScanLine() { return memory[LY_addr]; }
void MMU::incrementCurrentScanLine() { memory[LY_addr]++; }

bool MMU::isLYCEqualLY() { return memory[LY_addr] == memory[LYC_addr]; }

}  // namespace GB
