#include <algorithm>

#include "GB/PPU.hpp"

namespace GB {

void PPU::renderSprites() {
  sprite_buffer.clear();
  constexpr uint16_t max_sprites{40};
  constexpr size_t max_sprite_buffer_size{10};
  for (uint16_t sprite = 0; sprite < max_sprites; ++sprite) {
    if (sprite_buffer.size() >= max_sprite_buffer_size) {
      break;
    }
    uint16_t sprite_OAM_address = getOAMSpriteAddress(sprite);
    uint32_t raw_oam_entry = mmu->getOAMSpriteEntry(sprite);
    uint8_t y_position = getYPositionFromOAM(raw_oam_entry);
    uint8_t x_position = getXPositionFromOAM(raw_oam_entry);
    uint8_t tile_number = getTileNumberFromOAM(raw_oam_entry);
    uint8_t sprite_flags = getSpriteFlagsFromOAM(raw_oam_entry);

    if (isValidSpriteOAMEntry(y_position, x_position)) {
      SpriteOAMEntry entry;
      entry.y = y_position;
      entry.x = x_position;
      entry.tile_number = tile_number;
      entry.sprite_flags = sprite_flags;
      entry.entry_address = sprite_OAM_address;
      sprite_buffer.push_back(entry);
    }
  }

  sortSpriteBufferByPriority();

  for (const SpriteOAMEntry& sprite : sprite_buffer) {
    addSpriteToDisplayBuffer(sprite);
  }
}

uint16_t PPU::getOAMSpriteAddress(uint16_t offset) {
  constexpr uint16_t OAM_addr{0xFE00};
  return 4 * offset + OAM_addr;
}

uint8_t PPU::getYPositionFromOAM(uint32_t entry) { return (entry & 0xFF) - 16; }
uint8_t PPU::getXPositionFromOAM(uint32_t entry) {
  return ((entry >> 8) & 0xFF) - 8;
}
uint8_t PPU::getTileNumberFromOAM(uint32_t entry) {
  uint8_t tile_number = (entry >> 16) & 0xFF;

  // See: https://gbdev.io/pandocs/OAM.html
  if (mmu->isTallSpriteSizeSet()) {
    tile_number = tile_number & 0xFE;
  }
  return tile_number;
}
uint8_t PPU::getSpriteFlagsFromOAM(uint32_t entry) {
  return (entry >> 24) & 0xFF;
}

bool PPU::isValidSpriteOAMEntry(uint8_t y_position, uint8_t x_position) {
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

void PPU::sortSpriteBufferByPriority() {
  std::sort(sprite_buffer.begin(), sprite_buffer.end(), compareOAMEntry);
}

bool PPU::compareOAMEntry(const SpriteOAMEntry& left,
                          const SpriteOAMEntry& right) {
  if (left.x == right.x) {
    return left.entry_address > right.entry_address;
  }
  return left.x > right.x;
}

void PPU::addSpriteToDisplayBuffer(SpriteOAMEntry entry) {
  uint8_t tile_data_y_offset =
      getSpriteTileDataYOffset(entry.y, entry.sprite_flags);
  uint16_t tile_data_address = getSpriteTileDataAddress(entry.tile_number);
  uint8_t byte1 = mmu->read(tile_data_address + tile_data_y_offset);
  uint8_t byte2 = mmu->read(tile_data_address + tile_data_y_offset + 1);
  for (int8_t tile_pixel = 7; tile_pixel >= 0; tile_pixel--) {
    uint8_t colour_position =
        getSpriteColourPosition(tile_pixel, entry.sprite_flags);
    uint16_t colour_id = get2BPPPixel(byte1, byte2, colour_position);
    uint16_t colour_address = getSpriteColourAddress(entry.sprite_flags);
    Colour colour = decodeColour(colour_id, colour_address);
    uint8_t pixel = getSpritePixelLocation(entry.x, tile_pixel);
    uint8_t scanline = mmu->getCurrentScanLine();

    if (scanline >= lcd_viewport_height || pixel >= lcd_viewport_width) {
      continue;
    }

    if (shouldDrawPixel(entry.sprite_flags, colour_id, pixel, scanline)) {
      draw(pixel, scanline, colour, colour_id);
    }
  }
}

uint8_t PPU::getSpriteTileDataYOffset(uint8_t y_position,
                                      uint8_t sprite_flags) {
  uint8_t offset = mmu->getCurrentScanLine() - y_position;
  if (shouldSpriteYFlip(sprite_flags)) {
    if (mmu->isTallSpriteSizeSet()) {
      constexpr uint8_t sprite_size{16};
      offset = sprite_size - offset - 1;
    } else {
      constexpr uint8_t sprite_size{8};
      offset = sprite_size - offset - 1;
    }
  }
  constexpr uint8_t offset_byte_size{2};
  return offset_byte_size * offset;
}

uint16_t PPU::getSpriteTileDataAddress(uint8_t tile_number) {
  return mmu->getPPU8000MethodAddress(tile_number);
}

uint8_t PPU::getSpriteColourPosition(int8_t tile_pixel, uint8_t sprite_flags) {
  uint8_t colour_position = tile_pixel;
  if (!shouldSpriteXFlip(sprite_flags)) {
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

uint8_t PPU::getSpritePixelLocation(uint8_t x_position, int8_t tile_pixel) {
  return tile_pixel + x_position;
}

bool PPU::shouldDrawPixel(uint8_t sprite_flags, uint8_t colour_id,
                          uint8_t pixel, uint8_t scanline) {
  if (colour_id == 0) {
    return false;
  } else if (isBackgroundPrioritized(sprite_flags) &&
             display_buffer_colour_id[getFlattenedDisplayBufferColourIDIndex(
                 pixel, scanline)] != 0) {
    return false;
  }
  return true;
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
