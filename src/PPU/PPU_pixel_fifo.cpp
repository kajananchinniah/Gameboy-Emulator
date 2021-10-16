#include "GB/PPU.hpp"
namespace {
enum FIFOMode {
  FETCH_TILE_NO,
  FETCH_TILE_DATA_LOW,
  FETCH_TILE_DATA_HIGH,
  PUSH
};

void clearQueue(std::queue<uint8_t> &q) {
  std::queue<uint8_t> empty_queue;
  std::swap(q, empty_queue);
}

}  // namespace
namespace GB {

void PPU::initPixelFIFO() {
  clearQueue(background_fifo);
  clearQueue(sprite_fifo);
  background_fifo_x_count = 0;
}

void PPU::updatePixelFIFO(int cycles) {
  updateBackgroundFIFO(cycles);
  updateSpriteFIFO(cycles);
  pushtoLCD(cycleS);
}

void PPU::updateBackgroundFIFO(int cycles) {
  while (cycles > 0) {
    switch (background_fifo_mode) {
      case FETCH_TILE_NO:
        fetchBackgroundTileNo();
        cycles -= 2;
        break;
      case FETCH_TILE_DATA_LOW:
        fetchBackgroundTileDataLow();
        cycles -= 2;
        break;
      case FETCH_TILE_DATA_HIGH:
        fetchBackgroundTileDataHigh();
        cycles -= 2;
        break;
      case PUSH:
        pushToBackgroundFIFO();
        cycles -= 1;
        break;
    }
  }
}

void PPU::fetchBackgroundTileNo() {
  if (!window_fifo) {
    uint16_t tilemap_addr = 0x9800;
    if (mmu->isBGTileMapSelectSet()) {
      tilemap_addr = 0x9C00;
    }
    tilemap_addr += background_fifo_x_count;
    tilemap_addr += mmu->getScrollX() / 8;
    tilemap_addr = tilemap_addr & 0x1F;
    tilemap_addr +=
        32 * (((mmu->getCurrentScanLine() + mmu->getScrollY()) & 0xFF) / 8);
    current_background_tile_num = mmu->readTileNumber(tilemap_addr);
  } else {
    return;
  }
}

void PPU::fetchBackgroundTileDataLow() {
  if (!window_fifo) {
    background_low_data = 0;
  }
}

}  // namespace GB
