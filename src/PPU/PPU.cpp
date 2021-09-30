#include "GB/PPU.hpp"

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

uint8_t PPU::get2BPPPixel(uint8_t byte1, uint8_t byte2, int position) {
  uint8_t low = (byte1 >> position) & 0x01;
  uint8_t high = (byte2 >> position) & 0x01;
  return (high << 1) | low;
}

uint16_t PPU::get2BPPPixelRow(uint8_t byte1, uint8_t byte2) {
  uint16_t rval = 0x0000;
  for (int i = 0; i < 8; ++i) {
    rval = rval | (get2BPPpixel(byte1, byte2, i) << 2 * i);
  }
  return rval;
}

}  // namespace GB
