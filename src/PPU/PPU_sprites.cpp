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

    display_buffer[pixel][scanline][kRedDisplayBufferIndex] = colour.red;
    display_buffer[pixel][scanline][kGreenDisplayBufferIndex] = colour.green;
    display_buffer[pixel][scanline][kBlueDisplayBufferIndex] = colour.blue;
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
  return mmu->getPPU8000MethodAddress(tile_number) + line;
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

}  // namespace GB
