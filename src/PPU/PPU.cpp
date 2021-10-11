#include "GB/PPU.hpp"

namespace {

enum PPUModes { OAM_SCAN = 2, DRAWING = 3, H_BLANK = 0, V_BLANK = 1 };

}  // namespace

namespace GB {
PPU::PPU(MMU *mmu) { this->mmu = mmu; }
PPU::~PPU() {}

uint8_t PPU::getWindowHorizontalPosition() {
  uint8_t WX = mmu->getWindowXRegister();
  return WX - 0x7;
}

uint8_t PPU::getWindowVerticalPosition() {
  uint8_t WY = mmu->getWindowYRegister();
  return WY;
}

void PPU::updatePPU(int clock_cycles) {
  if (!mmu->isLCDDisplayEnabled()) {
    resetPPU();
    return;
  }

  ppu_clock_cycles += clock_cycles;
  updatePPULCD();
  if (ppu_clock_cycles >= end_of_scanline_cycles) {
    prepareForNextScanLine();
  }
}

void PPU::resetPPU() {
  ppu_clock_cycles = 0;
  mmu->setCurrentScanLine(0);
  mmu->setPPUMode(PPUModes::V_BLANK);
}

void PPU::updatePPULCD() {
  uint8_t ppu_mode = mmu->getPPUMode();
  switch (ppu_mode) {}
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
