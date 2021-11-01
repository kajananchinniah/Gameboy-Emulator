#include "GB/PPU.hpp"

#include <stdexcept>
#include <unordered_map>

namespace {

enum PPUModes { OAM_SCAN = 2, DRAWING = 3, H_BLANK = 0, V_BLANK = 1 };

const std::unordered_map<int32_t, GB::Colour> g_BGP_colour_palette = {
    {GB::Colour::ColourCode::WHITE,
     GB::Colour{0xFF, 0xFF, 0xFF, GB::Colour::ColourCode::WHITE}},

    {GB::Colour::ColourCode::LIGHT_GRAY,
     GB::Colour{0xAA, 0xAA, 0xAA, GB::Colour::ColourCode::LIGHT_GRAY}},

    {GB::Colour::ColourCode::DARK_GRAY,
     GB::Colour{0x55, 0x55, 0x55, GB::Colour::ColourCode::DARK_GRAY}},

    {GB::Colour::ColourCode::BLACK,
     GB::Colour{0x00, 0x00, 0x00, GB::Colour::ColourCode::BLACK}}};

const std::unordered_map<int32_t, GB::Colour> g_OBP0_colour_palette = {
    {GB::Colour::ColourCode::WHITE,
     GB::Colour{0xFF, 0xFF, 0xFF, GB::Colour::ColourCode::WHITE}},

    {GB::Colour::ColourCode::LIGHT_GRAY,
     GB::Colour{0xAA, 0xAA, 0xAA, GB::Colour::ColourCode::LIGHT_GRAY}},

    {GB::Colour::ColourCode::DARK_GRAY,
     GB::Colour{0x55, 0x55, 0x55, GB::Colour::ColourCode::DARK_GRAY}},

    {GB::Colour::ColourCode::BLACK,
     GB::Colour{0x00, 0x00, 0x00, GB::Colour::ColourCode::BLACK}}};

const std::unordered_map<int32_t, GB::Colour> g_OBP1_colour_palette = {
    {GB::Colour::ColourCode::WHITE,
     GB::Colour{0xFF, 0xFF, 0xFF, GB::Colour::ColourCode::WHITE}},

    {GB::Colour::ColourCode::LIGHT_GRAY,
     GB::Colour{0xAA, 0xAA, 0xAA, GB::Colour::ColourCode::LIGHT_GRAY}},

    {GB::Colour::ColourCode::DARK_GRAY,
     GB::Colour{0x55, 0x55, 0x55, GB::Colour::ColourCode::DARK_GRAY}},

    {GB::Colour::ColourCode::BLACK,
     GB::Colour{0x00, 0x00, 0x00, GB::Colour::ColourCode::BLACK}}};

}  // namespace

namespace GB {
PPU::PPU(MMU *mmu) { this->mmu = mmu; }
PPU::~PPU() {}

int PPU::getDisplayBufferPitch() {
  return lcd_viewport_width * sizeof(uint8_t) * num_display_buffer_channels;
}

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

  updatePPULCD();
  ppu_clock_cycles += clock_cycles;
  if (ppu_clock_cycles >= end_of_scanline_cycles) {
    prepareForNextScanLine();
  }
}

void PPU::resetPPU() {
  ppu_clock_cycles = 0;
  mmu->setCurrentScanLine(0);
  mmu->setPPUMode(PPUModes::H_BLANK);
  requested_coincidence_interrupt = false;
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
  if (ppu_clock_cycles >= mode_2_cycles) {
    mmu->setPPUMode(PPUModes::DRAWING);
    if (mmu->isMode2STATInterruptEnabled()) {
      mmu->setLCDStatInterruptRequest();
    }
  }
}

void PPU::doDrawingMode() {
  if (ppu_clock_cycles >= mode_3_cycles) {
    mmu->setPPUMode(PPUModes::H_BLANK);
  }
}

void PPU::doHBlankMode() {
  if (ppu_clock_cycles >= end_of_scanline_cycles) {
    if (mmu->isMode0STATInterruptEnabled()) {
      mmu->setLCDStatInterruptRequest();
    }

    if (mmu->getCurrentScanLine() < lcd_viewport_height) {
      mmu->setPPUMode(PPUModes::OAM_SCAN);
    } else if (mmu->getCurrentScanLine() == lcd_viewport_height) {
      mmu->setPPUMode(PPUModes::V_BLANK);
    }
  }
}

void PPU::doVBlankMode() {
  if (mmu->getCurrentScanLine() > max_scanline) {
    mmu->setPPUMode(PPUModes::OAM_SCAN);
    if (mmu->isMode1STATInterruptEnabled()) {
      mmu->setLCDStatInterruptRequest();
    }
  }
}

void PPU::updateCoincidenceFlag() {
  if (mmu->isLYCEqualLY()) {
    mmu->setCoincidenceFlag();
    if (mmu->isLYCEqualLYSTATInterruptEnabled() &&
        !requested_coincidence_interrupt) {
      mmu->setLCDStatInterruptRequest();
      requested_coincidence_interrupt = true;
    }
  }
}

void PPU::prepareForNextScanLine() {
  mmu->incrementCurrentScanLine();
  ppu_clock_cycles = ppu_clock_cycles - 456;
  requested_coincidence_interrupt = false;
  mmu->resetCoincidenceFlag();
  if (mmu->getCurrentScanLine() == lcd_viewport_height) {
    internal_window_counter = 0;
    mmu->setVBlankInterruptRequest();
  } else if (mmu->getCurrentScanLine() > max_scanline) {
    mmu->setCurrentScanLine(0);
  } else if (mmu->getCurrentScanLine() < lcd_viewport_height) {
    renderScanLine();
  }
}

void PPU::renderScanLine() {
  if (mmu->isBGAndWindowEnabled()) {
    renderBackground();
    renderWindow();
  }

  if (mmu->isSpritesEnabled()) {
    renderSprites();
  }
}

Colour PPU::decodeColour(uint8_t colour_id, uint16_t palette_addr) {
  uint8_t palette = mmu->read(palette_addr);
  int decoded_colour{0};
  switch (colour_id) {
    case 0:
      decoded_colour = getBit(1, palette) << 1 | getBit(0, palette);
      break;
    case 1:
      decoded_colour = getBit(3, palette) << 1 | getBit(2, palette);
      break;
    case 2:
      decoded_colour = getBit(5, palette) << 1 | getBit(4, palette);
      break;
    case 3:
      decoded_colour = getBit(7, palette) << 1 | getBit(6, palette);
      break;
  }
  return getRGBColour(decoded_colour, palette_addr);
}

Colour PPU::getRGBColour(int decoded_colour, uint16_t palette_addr) {
  switch (palette_addr) {
    case BGP_addr:
      return g_BGP_colour_palette.at(decoded_colour);
    case OBP0_addr:
      return g_OBP0_colour_palette.at(decoded_colour);
    case OBP1_addr:
      return g_OBP1_colour_palette.at(decoded_colour);
    default:
      throw std::runtime_error("Received invalid palette address!");
  }
}

uint8_t PPU::get2BPPPixel(uint8_t byte1, uint8_t byte2, int position) {
  uint8_t low = (byte1 >> position) & 0x01;
  uint8_t high = (byte2 >> position) & 0x01;
  return (high << 1) | low;
}

bool PPU::shouldUse8000Mode() {
  if (mmu->isTileDataSelectSet()) {
    return true;
  } else {
    return false;
  }
}

uint16_t PPU::get2BPPPixelRow(uint8_t byte1, uint8_t byte2) {
  uint16_t rval = 0x0000;
  for (int i = 0; i < 8; ++i) {
    rval = rval | (get2BPPPixel(byte1, byte2, i) << (2 * i));
  }
  return rval;
}

size_t PPU::getFlattenedIndex(uint8_t pixel, uint8_t scanline) {
  return scanline * lcd_viewport_width * num_display_buffer_channels +
         pixel * num_display_buffer_channels;
}

void PPU::draw(uint8_t pixel, uint8_t scanline, Colour colour,
               uint8_t colour_id) {
  size_t effective_idx = getFlattenedIndex(pixel, scanline);
  display_buffer[effective_idx + kAlphaDisplayBufferIndex] = 0xFF;
  display_buffer[effective_idx + kRedDisplayBufferIndex] = colour.red;
  display_buffer[effective_idx + kGreenDisplayBufferIndex] = colour.green;
  display_buffer[effective_idx + kBlueDisplayBufferIndex] = colour.blue;
  display_buffer_colour_id[pixel][scanline] = colour_id;
}

}  // namespace GB
