#include "GB/PPU.hpp"

#include <unordered_map>

namespace {

enum PPUModes { OAM_SCAN = 2, DRAWING = 3, H_BLANK = 0, V_BLANK = 1 };
enum ColourCode { WHITE, LIGHT_GRAY, DARK_GRAY, BLACK };

const std::unordered_map<int32_t, GB::Colour> colour_palette = {
    {ColourCode::WHITE, GB::Colour{0xFF, 0xFF, 0xFF, ColourCode::WHITE}},

    {ColourCode::LIGHT_GRAY,
     GB::Colour{0xCC, 0xCC, 0xCC, ColourCode::LIGHT_GRAY}},

    {ColourCode::DARK_GRAY,
     GB::Colour{0x77, 0x77, 0x77, ColourCode::DARK_GRAY}},

    {ColourCode::BLACK, GB::Colour{0x00, 0x00, 0x00, ColourCode::BLACK}}};

constexpr int kRedIndex{0};
constexpr int kGreenIndex{1};
constexpr int kBlueIndex{2};

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
  if (ppu_clock_cycles >= end_of_scanline_cycles) {
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

void PPU::doVBlankMode() { return; }

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
  } else if (mmu->getCurrentScanLine() > max_scanline) {
    mmu->setPPUMode(PPUModes::OAM_SCAN);
    mmu->setCurrentScanLine(0);
  } else if (mmu->getCurrentScanLine() < lcd_viewport_height) {
    renderScanLine();
  }
}

void PPU::renderScanLine() {
  if (mmu->isBGAndWindowEnabled()) {
    renderTiles();
  }

  if (mmu->isSpritesEnabled()) {
    renderSprites();
  }
}

void PPU::renderTiles() {
  uint8_t y_position = getBGAndWindowYPosition();
  uint16_t tile_row = getBGAndWindowTileRow(y_position);
  for (unsigned int pixel = 0; pixel < lcd_viewport_width; ++pixel) {
    uint8_t x_position = getBGAndWindowXPosition(static_cast<uint8_t>(pixel));
    uint16_t tile_column = getBGAndWindowTileColumn(x_position);
    uint8_t tile_number =
        mmu->read(getBackgroundMemoryAddress() + tile_row + tile_column);

    uint16_t tile_address;
    if (shouldUse8000Mode()) {
      tile_address = mmu->getPPU8000MethodAddress(tile_number);
    } else {
      tile_address = mmu->getPPU8800MethodAddress(tile_number);
    }

    uint8_t line = getCurrentVerticalBGAndVerticalLine(y_position);
    uint8_t byte1 = mmu->read(tile_address + line);
    uint8_t byte2 = mmu->read(tile_address + line + 1);
    uint8_t colour_position = getBGAndWindowColourPosition(x_position);
    uint8_t colour_id = get2BPPPixel(byte1, byte2, colour_position);
    Colour colour = decodeColour(colour_id, 0xFF47);

    uint8_t scanline = mmu->getCurrentScanLine();
    // Note: pixel is guaranteed to lie within the viewable area
    if (scanline >= lcd_viewport_height) {
      continue;
    }

    display_buffer[pixel][scanline][kRedIndex] = colour.red;
    display_buffer[pixel][scanline][kGreenIndex] = colour.green;
    display_buffer[pixel][scanline][kBlueIndex] = colour.blue;
  }
}

uint8_t PPU::getBGAndWindowYPosition() {
  if (shouldUseWindow()) {
    return mmu->getCurrentScanLine() - getWindowVerticalPosition();
  } else {
    return mmu->getScrollYRegister() + mmu->getCurrentScanLine();
  }
}

uint16_t PPU::getBGAndWindowTileRow(uint8_t y_position) {
  return 32 * (y_position / 8);
}

uint8_t PPU::getBGAndWindowXPosition(uint8_t pixel) {
  if (shouldUseWindow() && pixel >= getWindowHorizontalPosition()) {
    return pixel - getWindowHorizontalPosition();
  } else {
    return pixel + mmu->getScrollXRegister();
  }
}

uint16_t PPU::getBGAndWindowTileColumn(uint8_t x_position) {
  return x_position / 8;
}

uint8_t PPU::getCurrentVerticalBGAndVerticalLine(uint8_t y_position) {
  return 2 * (y_position % 8);
}

uint8_t PPU::getBGAndWindowColourPosition(uint8_t x_position) {
  uint8_t colour_position = x_position % 8;

  // Note: pixel 0 -> position 7, pixel 1 -> position 6, etc
  // Can be modeled using the eqn: f(x) = -x + 7
  colour_position = -1 * colour_position + 7;
  return colour_position;
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
  return colour_palette.at(decoded_colour);
}

bool PPU::shouldUseWindow() {
  if (mmu->isWindowDisplayEnabled() &&
      getWindowVerticalPosition() <= mmu->getCurrentScanLine()) {
    return true;
  } else {
    return false;
  }
}

bool PPU::shouldUse8000Mode() {
  if (mmu->isTileDataSelectSet()) {
    return true;
  } else {
    return false;
  }
}

uint16_t PPU::getBackgroundMemoryAddress() {
  if (shouldUseWindow()) {
    if (mmu->isWindowTileMapSelectSet()) {
      return 0x9C00;
    } else {
      return 0x9800;
    }
  } else {
    if (mmu->isBGTileMapSelectSet()) {
      return 0x9C00;
    } else {
      return 0x9800;
    }
  }
}

void PPU::renderSprites() {
  constexpr uint16_t max_sprites{40};
  for (uint16_t sprite = 0; sprite < max_sprites; ++sprite) {
    uint32_t raw_oam_entry = mmu->getOAMSpriteEntry(sprite);
    uint8_t y_position = getYPositionFromOAM(raw_oam_entry);
    uint8_t x_position = getXPositionFromOAM(raw_oam_entry);
    uint8_t tile_number = getTileNumberFromOAM(raw_oam_entry);
    uint8_t sprite_flags = getSpriteFlagsFromOAM(raw_oam_entry);

    if (isValidSpriteOAMEntry(y_position, x_position, tile_number,
                              sprite_flags)) {
      addSpriteToDisplayBuffer(y_position, x_position, tile_number,
                               sprite_flags);
    }
  }
}

uint8_t PPU::getYPositionFromOAM(uint32_t entry) { return (entry & 0xFF) - 16; }
uint8_t PPU::getXPositionFromOAM(uint32_t entry) {
  return ((entry >> 8) & 0xFF) - 8;
}
uint8_t PPU::getTileNumberFromOAM(uint32_t entry) {
  return (entry >> 16) & 0xFF;
}
uint8_t PPU::getSpriteFlagsFromOAM(uint32_t entry) {
  return (entry >> 24) & 0xFF;
}

bool PPU::isValidSpriteOAMEntry(uint8_t y_position, uint8_t x_position,
                                uint8_t tile_number, uint8_t sprite_flags) {
  bool validity = (mmu->getCurrentScanLine()) >= y_position;
  if (mmu->isTallSpriteSizeSet()) {
    validity = validity && (mmu->getCurrentScanLine()) < y_position + 16;
  } else {
    validity = validity && (mmu->getCurrentScanLine()) < y_position + 8;
  }
  return validity;
}

void PPU::addSpriteToDisplayBuffer(uint8_t y_position, uint8_t x_position,
                                   uint8_t tile_number, uint8_t sprite_flags) {
  uint8_t line = getSpriteVerticalLine(y_position, sprite_flags);
  uint16_t data_address = getSpriteDataAddress(tile_number, line);
  uint8_t byte1 = mmu->read(data_address);
  uint8_t byte2 = mmu->read(data_address + 1);
  for (int8_t tile_pixel = 7; tile_pixel >= 0; tile_pixel--) {
    uint8_t colour_position = getSpriteColourPosition(tile_pixel, sprite_flags);
    uint16_t colour_id = get2BPPPixel(byte1, byte2, colour_position);
    uint16_t colour_addr = getSpriteColourAddress(sprite_flags);
    Colour colour = decodeColour(colour_id, colour_addr);
    if (colour.colour == ColourCode::WHITE) {
      continue;
    }

    int pixel = getSpritePixelLocation(x_position, tile_pixel);
    uint8_t scanline = mmu->getCurrentScanLine();
    // Note: we need to ensure that the pixel is in the viewable area
    if (scanline >= lcd_viewport_height || pixel < 0 ||
        pixel >= static_cast<int>(lcd_viewport_width)) {
      continue;
    }

    display_buffer[pixel][scanline][kRedIndex] = colour.red;
    display_buffer[pixel][scanline][kGreenIndex] = colour.green;
    display_buffer[pixel][scanline][kBlueIndex] = colour.blue;
  }
}

uint8_t PPU::getSpriteVerticalLine(uint8_t y_position, uint8_t sprite_flags) {
  uint8_t line = mmu->getCurrentScanLine() - y_position;
  if (checkBit(6, sprite_flags)) {
    if (mmu->isTallSpriteSizeSet()) {
      line = 16 - line;
    } else {
      line = 8 - line;
    }
  }
  return 2 * line;
}

uint16_t PPU::getSpriteDataAddress(uint8_t tile_number, uint8_t line) {
  return 0x8000 + (tile_number * 16) + line;
}

uint8_t PPU::getSpriteColourPosition(int8_t tile_pixel, uint8_t sprite_flags) {
  uint8_t colour_position = tile_pixel;
  if (checkBit(5, sprite_flags)) {
    colour_position = 7 - colour_position;
  }
  return colour_position;
}

uint16_t PPU::getSpriteColourAddress(uint8_t sprite_flags) {
  if (checkBit(4, sprite_flags)) {
    return 0xFF49;
  } else {
    return 0xFF48;
  }
}

int PPU::getSpritePixelLocation(uint8_t x_position, int8_t tile_pixel) {
  return 7 - tile_pixel + x_position;
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
