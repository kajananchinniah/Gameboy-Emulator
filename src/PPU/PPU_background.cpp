#include "GB/PPU.hpp"

namespace GB {

void PPU::renderBackground() {
  uint16_t background_tile_y_offset = getBackgroundTileNumberYOffset();
  for (uint8_t pixel = 0; pixel < lcd_viewport_width; ++pixel) {
    uint16_t background_tile_x_offset = getBackgroundTileNumberXOffset(pixel);
    uint8_t tile_number =
        mmu->read(getBackgroundMemoryAddress() + background_tile_x_offset +
                  background_tile_y_offset);
    uint16_t tile_address;
    if (shouldUse8000Mode()) {
      tile_address = mmu->getPPU8000MethodAddress(tile_number);
    } else {
      tile_address = mmu->getPPU8800MethodAddress(tile_number);
    }

    uint16_t tile_data_offset_y = getBackgroundTileDataYOffset();
    uint8_t byte1 = mmu->read(tile_address + tile_data_offset_y);
    uint8_t byte2 = mmu->read(tile_address + tile_data_offset_y + 1);
    uint16_t tile_data_offset_x = getBackgroundTileDataXOffset(pixel);
    uint8_t colour_position = getBackgroundColourPosition(tile_data_offset_x);
    uint8_t colour_id = get2BPPPixel(byte1, byte2, colour_position);
    Colour colour = decodeColour(colour_id, BGP_addr);

    uint8_t scanline = mmu->getCurrentScanLine();
    if (scanline >= lcd_viewport_height) {
      continue;
    }

    display_buffer[pixel][scanline][kRedDisplayBufferIndex] = colour.red;
    display_buffer[pixel][scanline][kGreenDisplayBufferIndex] = colour.green;
    display_buffer[pixel][scanline][kBlueDisplayBufferIndex] = colour.blue;
    display_buffer_colour[pixel][scanline] = colour;
  }
}

uint16_t PPU::getBackgroundTileNumberYOffset() {
  uint16_t offset =
      (mmu->getCurrentScanLine() + mmu->getScrollYRegister()) & 0xFF;
  offset = offset / 8;
  return 32 * offset;
}

uint16_t PPU::getBackgroundTileNumberXOffset(uint8_t pixel) {
  uint16_t offset = (pixel + mmu->getScrollXRegister()) & 0xFF;
  offset = offset / 8;
  return offset & 0x1F;
}

uint16_t PPU::getBackgroundMemoryAddress() {
  if (mmu->isBGTileMapSelectSet()) {
    return background_map1_addr;
  } else {
    return background_map2_addr;
  }
}

uint16_t PPU::getBackgroundTileDataYOffset() {
  return 2 *
         (((mmu->getCurrentScanLine() + mmu->getScrollYRegister()) & 0xFF) % 8);
}

uint16_t PPU::getBackgroundTileDataXOffset(uint8_t pixel) {
  return ((pixel + mmu->getScrollXRegister()) & 0xFF) % 8;
}

uint8_t PPU::getBackgroundColourPosition(uint16_t data) {
  return 7 - (data & 0xFF);
}

}  // namespace GB
