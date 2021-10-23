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
    Colour colour = decodeColour(colour_id, 0xFF47);

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

}  // namespace GB
