#include "GB/PPU.hpp"

namespace GB {

void PPU::renderWindow() {
  if (!shouldRenderWindow()) {
    return;
  }

  uint16_t window_tile_number_y_offset = getWindowTileNumberYOffset();
  // Note: horizontal window position represents the starting x position of the
  // rectangle window
  for (uint8_t pixel = getWindowHorizontalPosition();
       pixel < lcd_viewport_width; ++pixel) {
    uint16_t window_tile_number_x_offset = getWindowTileNumberXOffset(pixel);
    uint8_t tile_number =
        mmu->read(getWindowMemoryAddress() + window_tile_number_x_offset +
                  window_tile_number_y_offset);
    uint16_t tile_address;
    if (shouldUse8000Mode()) {
      tile_address = mmu->getPPU8000MethodAddress(tile_number);
    } else {
      tile_address = mmu->getPPU8800MethodAddress(tile_number);
    }

    uint16_t tile_data_y_offset = getWindowTileDataYOffset();
    uint8_t byte1 = mmu->read(tile_address + tile_data_y_offset);
    uint8_t byte2 = mmu->read(tile_address + tile_data_y_offset + 1);
    uint16_t tile_data_x_offset = getWindowTileDataXOffset(pixel);
    uint8_t colour_position = getWindowColourPosition(tile_data_x_offset);
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
  internal_window_counter++;
}

bool PPU::shouldRenderWindow() {
  if (mmu->isWindowDisplayEnabled() &&
      getWindowVerticalPosition() <= mmu->getCurrentScanLine() &&
      getWindowHorizontalPosition() < lcd_viewport_width) {
    return true;
  } else {
    return false;
  }
}

uint16_t PPU::getWindowTileNumberYOffset() {
  return 32 * (internal_window_counter / 8);
}

uint16_t PPU::getWindowTileNumberXOffset(uint8_t pixel) {
  return (pixel - getWindowHorizontalPosition()) / 8;
}

uint16_t PPU::getWindowMemoryAddress() {
  if (mmu->isWindowTileMapSelectSet()) {
    return background_map1_addr;
  } else {
    return background_map2_addr;
  }
}

uint16_t PPU::getWindowTileDataYOffset() {
  return 2 * (internal_window_counter % 8);
}

uint16_t PPU::getWindowTileDataXOffset(uint8_t pixel) {
  return (pixel - getWindowHorizontalPosition()) % 8;
}

uint8_t PPU::getWindowColourPosition(uint16_t data) {
  return 7 - (data & 0xFF);
}

}  // namespace GB
