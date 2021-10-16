#include "GB/PPU.hpp"
namespace {

enum PPUMode { OAM_SCAN = 2, DRAWING = 3, H_BLANK = 0, V_BLANK = 1 };

}  // namespace

namespace GB {
PPU::PPU(MMU *mmu) { this->mmu = mmu; }
PPU::~PPU() {}

uint8_t PPU::getWindowHorizontalPosition() {
  uint8_t WX = mmu->getWindowXRegister();
  return WX - 0x7;
}

void PPU::updatePPU(int clock_cycles) {
  if (!mmu->isLCDDisplayEnabled()) {
    resetPPU();
    return;
  }

  updatePPULCD(clock_cycles);

  ppu_clock_cycles += clock_cycles;
  if (ppu_clock_cycles >= kMaxPPUClockCycles) {
    requestPPUInterrupt();
  }
}

void PPU::updatePPULCD(int clock_cycles) {
  uint8_t ppu_mode = mmu->getPPUMode();
  uint8_t current_scanline = mmu->getCurrentScanLine();
  switch (ppu_mode) {
    case OAM_SCAN:
      performOAMScanMode(clock_cycles);
      break;
    case DRAWING:
      performDrawingMode(clock_cycles);
      break;
    case H_BLANK:
      performHBlankMode(clock_cycles);
      break;
    case V_BLANK:
      performVBlankMode(clock_cycles);
      break;
  }

  if (mmu->isLYEqualLYC()) {
    mmu->setCoincidenceFlag();
    if (mmu->isLYCEqualLYSTATInterruptEnabled()) {
      mmu->setLCDStatInterruptRequest();
    }
  } else {
    mmu->resetCoincidenceFlag();
  }
}

void PPU::performOAMScanMode(int clock_cycles) {
  while (clock_cycles > 0) {
    uint32_t raw_oam_entry = mmu->getOAMSpriteEntry(oam_sprite_number);
    uint8_t y_position = getXPositionFromOAM(raw_oam_entry);
    uint8_t x_position = getYPositionFromOAM(raw_oam_entry);
    uint8_t tile_number = getTileNumberFromOAM(raw_oam_entry);
    uint8_t sprite_flags = getSpriteFlagsFromOAM(raw_oam_entry);
    if (isValidSpriteOAMEntry(y_position, x_position, tile_number,
                              sprite_flags)) {
      OAMEntry entry{y_position, x_position, tile_number, sprite_flags};
      sprite_buffer.push_back(entry);
    }
    oam_sprite_number++;
    clock_cycles -= 2;
  }

  if (ppu_clock_cycles == kOAMScanCycles) {
    switchPPUMode(DRAWING);
  }
}

void PPU::switchPPUMode(uint8_t new_mode) {
  switch (new_mode) {
    case DRAWING: {
      mmu->setPPUMode(DRAWING);
      pixel_fifo->init();
      if (mmu->isMode2STATInterruptEnabled()) {
        mmu->setLCDStatInterruptRequest();
      }
    }
  }
}

uint8_t getYPositionFromOAM(uint32_t entry) { return (entry & 0xFF); }
uint8_t getXPositionFromOAM(uint32_t entry) { return ((entry >> 8) & 0xFF); }
uint8_t getTileNumberFromOAM(uint32_t entry) { return (entry >> 16) & 0xFF; }
uint8_t getSpriteFlagsFromOAM(uint32_t entry) { return (entry >> 24) & 0xFF; }
bool PPU::isValidSpriteOAMEntry(uint8_t x_pos, uint8_t y_pos, uint8_t tile_num,
                                uint8_t sprite_flags) {
  bool value = x_pos > 0;
  value = value && (mmu->getCurrentScanLine() + 16) >= y_pos;
  if (mmu->isTallSpriteSizeSet()) {
    value = value && (mmu->getCurrentScanLine() + 16) < y_pos + 16;
  } else {
    value = value && (mmu->getCurrentScanLine() + 16) < y_pos + 8;
  }
  value = value && sprite_buffer.size() < 10;
  return value;
}

uint8_t PPU::getWindowVerticalPosition() {
  uint8_t WY = mmu->getWindowYRegister();
  return WY;
}

uint8_t PPU::get2BPPPixel(uint8_t byte1, uint8_t byte2, int position) {
  uint8_t low = (byte1 >> position) & 0x01;
  uint8_t high = (byte2 >> position) & 0x01;
  return (high << 1) | low;
}

uint16_t PPU::get2BPPPixelRow(uint8_t byte1, uint8_t byte2) {
  uint16_t rval = 0x0000;
  for (int i = 0; i < 8; ++i) {
    rval = rval | (get2BPPPixel(byte1, byte2, i) << (2 * i));
  }
  return rval;
}

}  // namespace GB
