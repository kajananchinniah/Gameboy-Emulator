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
  switch (ppu_mode) {
    case PPUModes::OAM_SCAN:
      doOAMScanMode();
      break;
    case PPUModes::DRAWING:
      doDrawingMode();
      break;
    case PPUModes::H_BLANK:
      doHBlankMode();
      break;
    case PPUModes::V_BLANK:
      doVBlankMode();
      break;
  }
  updateCoincidenceFlag();
}

void PPU::doOAMScanMode() {
  if (ppu_clock_cycles < 80) {
    mmu->setPPUMode(PPUModes::DRAWING);
    if (mmu->isMode2STATInterruptEnabled()) {
      mmu->setLCDStatInterruptRequest();
    }
  }
}

void PPU::doDrawingMode() {
  if (ppu_clock_cycles < 252) {
    mmu->setPPUMode(PPUModes::H_BLANK);
  }
}

void PPU::doHBlankMode() {
  if (ppu_clock_cycles >= max_ppu_cycles) {
    if (mmu->isMode0STATInterruptEnabled()) {
      mmu->setLCDStatInterruptRequest();
    }

    if (mmu->getCurrentScanLine() < lcd_viewport_height) {
      mmu->setPPUMode(PPUModes::OAM_SCAN);
    } else {
      mmu->setPPUMode(PPUModes::V_BLANK);
    }
  }
}

void PPU::updateCoincidenceFlag() {
  if (mmu->isLYCEqualLY()) {
    mmu->setCoincidenceFlag();
    if (mmu->isLYCEqualLYSTATInterruptEnabled()) {
      mmu->setLCDStatInterruptRequest();
    }
  } else {
    mmu->resetCoincidenceFlag();
  }
}

void PPU::prepareForNextScanLine() {
  mmu->incrementCurrentScanLine();
  ppu_clock_cycles = 0;
  if (mmu->getCurrentScanLine() == lcd_viewport_height) {
    mmu->setPPUMode(PPUModes::V_BLANK);
    mmu->setVBlankInterruptRequest();
    return;
  }

  if (mmu->getCurrentScanLine() > max_scanline) {
    mmu->setPPUMode(PPUModes::OAM_SCAN);
    mmu->setCurrentScanLine(0);
    return;
  }
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
