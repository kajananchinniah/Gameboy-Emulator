#include "GB/PPU.hpp"

namespace GB {

void PPU::renderSprites() {
  constexpr uint16_t max_sprites{40};
  for (uint16_t sprite = 0; sprite < max_sprites; ++sprite) {
    uint32_t raw_oam_entry = mmu->getOAMSpriteEntry(sprite);
    uint8_t y_position = getYPositionFromOAM(raw_oam_entry);
    uint8_t x_position = getXPositionFromOAM(raw_oam_entry);
    uint8_t tile_number = getTileNumberFromOAM(raw_oam_entry);
    uint8_t sprite_flags = getSpriteFlagsFromOAM(raw_oam_entry);

    if (isValidSpriteOAMEntry(y_position, x_position)) {
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

bool PPU::isValidSpriteOAMEntry(uint8_t y_position, uint8_t x_position) {
  // Logic combined these two sources:
  // http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html +
  // https://hacktixme.ga/GBEDG/ppu/
  bool validity = x_position > 0;
  validity = validity && (mmu->getCurrentScanLine()) >= y_position;
  if (mmu->isTallSpriteSizeSet()) {
    constexpr uint8_t sprite_size{16};
    validity =
        validity && (mmu->getCurrentScanLine()) < y_position + sprite_size;
  } else {
    constexpr uint8_t sprite_size{8};
    validity =
        validity && (mmu->getCurrentScanLine()) < y_position + sprite_size;
  }
  return validity;
}

void PPU::addSpriteToDisplayBuffer(uint8_t y_position, uint8_t x_position,
                                   uint8_t tile_number, uint8_t sprite_flags) {
  uint8_t line = getSpriteVerticalLine(y_position, sprite_flags);
  uint16_t data_address = getSpriteDataAddress(tile_number, line);
  uint8_t byte1 = mmu->read(data_address);
  uint8_t byte2 = mmu->read(data_address + 1);
  // 8 bits in 1 byte; go reverse order because bit 7 -> pixel 0
  for (int8_t tile_pixel = 7; tile_pixel >= 0; tile_pixel--) {
    uint8_t colour_position = getSpriteColourPosition(tile_pixel, sprite_flags);
    uint16_t colour_id = get2BPPPixel(byte1, byte2, colour_position);
    uint16_t colour_addr = getSpriteColourAddress(sprite_flags);
    Colour colour = decodeColour(colour_id, colour_addr);
    if (colour.colour == Colour::ColourCode::WHITE) {
      continue;
    }

    int pixel = getSpritePixelLocation(x_position, tile_pixel);
    uint8_t scanline = mmu->getCurrentScanLine();
    // Note: we need to ensure that the pixel is in the viewable area
    if (scanline >= lcd_viewport_height || pixel < 0 ||
        pixel >= static_cast<int>(lcd_viewport_width)) {
      continue;
    }

    if (!isBackgroundPrioritized(sprite_flags) &&
        display_buffer_colour[pixel][scanline].colour !=
            Colour::ColourCode::WHITE) {
      display_buffer[pixel][scanline][kRedDisplayBufferIndex] = colour.red;
      display_buffer[pixel][scanline][kGreenDisplayBufferIndex] = colour.green;
      display_buffer[pixel][scanline][kBlueDisplayBufferIndex] = colour.blue;
      display_buffer_colour[pixel][scanline] = colour;
    }
  }
}

uint8_t PPU::getSpriteVerticalLine(uint8_t y_position, uint8_t sprite_flags) {
  uint8_t line = mmu->getCurrentScanLine() - y_position;
  if (shouldSpriteYFlip(sprite_flags)) {
    if (mmu->isTallSpriteSizeSet()) {
      constexpr uint8_t sprite_size{16};
      line = sprite_size - line;
    } else {
      constexpr uint8_t sprite_size{8};
      line = sprite_size - line;
    }
  }
  constexpr uint8_t line_byte_size{2};
  return line_byte_size * line;
}

uint16_t PPU::getSpriteDataAddress(uint8_t tile_number, uint8_t line) {
  return mmu->getPPU8000MethodAddress(tile_number) + line;
}

uint8_t PPU::getSpriteColourPosition(int8_t tile_pixel, uint8_t sprite_flags) {
  uint8_t colour_position = tile_pixel;
  if (shouldSpriteXFlip(sprite_flags)) {
    // 8 bits in one byte
    colour_position = 7 - colour_position;
  }
  return colour_position;
}

uint16_t PPU::getSpriteColourAddress(uint8_t sprite_flags) {
  if (shouldUsePalette1(sprite_flags)) {
    return OBP1_addr;
  } else {
    return OBP0_addr;
  }
}

int PPU::getSpritePixelLocation(uint8_t x_position, int8_t tile_pixel) {
  return 7 - tile_pixel + x_position;
}

bool PPU::shouldUsePalette1(uint8_t sprite_flags) {
  return checkBit(4, sprite_flags);
}

bool PPU::shouldSpriteXFlip(uint8_t sprite_flags) {
  return checkBit(5, sprite_flags);
}

bool PPU::shouldSpriteYFlip(uint8_t sprite_flags) {
  return checkBit(6, sprite_flags);
}

bool PPU::isBackgroundPrioritized(uint8_t sprite_flags) {
  return checkBit(7, sprite_flags);
}

}  // namespace GB
