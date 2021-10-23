#include "GB/PPU.hpp"

namespace GB {

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
    Colour colour = decodeColour(colour_id, BGP_addr);

    uint8_t scanline = mmu->getCurrentScanLine();
    // Note: pixel is guaranteed to lie within the viewable area
    if (scanline >= lcd_viewport_height) {
      continue;
    }

    display_buffer[pixel][scanline][kRedDisplayBufferIndex] = colour.red;
    display_buffer[pixel][scanline][kGreenDisplayBufferIndex] = colour.green;
    display_buffer[pixel][scanline][kBlueDisplayBufferIndex] = colour.blue;
  }
}

uint8_t PPU::getBGAndWindowYPosition() {
  // TODO: this should use the WINDOW_LINE_COUNTER
  // Logic for window used from:
  // http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html
  if (shouldUseWindow()) {
    return mmu->getCurrentScanLine() - getWindowVerticalPosition();
  } else {
    return mmu->getScrollYRegister() + mmu->getCurrentScanLine();
  }
}

uint16_t PPU::getBGAndWindowTileRow(uint8_t y_position) {
  // See: background pixel fetching (https://hacktixme.ga/GBEDG/ppu/)
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
  // Determine which tile it is; relies on rounding down behavior of int
  // E.g. 255 / 8 = 31.875 = 31 (round down)
  return x_position / 8;
}

uint8_t PPU::getCurrentVerticalBGAndVerticalLine(uint8_t y_position) {
  // See: background pixel fetching (https://hacktixme.ga/GBEDG/ppu/)
  return 2 * (y_position % 8);
}

uint8_t PPU::getBGAndWindowColourPosition(uint8_t x_position) {
  uint8_t colour_position = x_position % 8;

  // Note: pixel 0 -> position 7, pixel 1 -> position 6, etc
  // Can be modeled using the eqn: f(x) = 7 - x
  return 7 - colour_position;
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
  // See: PPU Registers (https://hacktixme.ga/GBEDG/ppu/)
  if (mmu->isTileDataSelectSet()) {
    return true;
  } else {
    return false;
  }
}

uint16_t PPU::getBackgroundMemoryAddress() {
  // See: PPU Registers (https://hacktixme.ga/GBEDG/ppu/)
  if (shouldUseWindow()) {
    if (mmu->isWindowTileMapSelectSet()) {
      return background_map1_addr;
    } else {
      return background_map2_addr;
    }
  } else {
    if (mmu->isBGTileMapSelectSet()) {
      return background_map1_addr;
    } else {
      return background_map2_addr;
    }
  }
}

}  // namespace GB
