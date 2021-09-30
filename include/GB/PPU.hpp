#ifndef INCLUDE_GB_PPU_HPP_
#define INCLUDE_GB_PPU_HPP_

#include "MMU.hpp"

namespace GB {
class PPU {
 public:
  explicit PPU(MMU *mmu);
  ~PPU();

 private:
  const unsigned int lcd_viewport_width{160};
  const unsigned int lcd_viewport_height{144};
  const unsigned int background_width{256};
  const unsigned int background_height{256};
  const unsigned int window_width{256};
  const unsigned int window_height{256};
  MMU *mmu;

  uint8_t getWindowHorizontalPosition();
  uint8_t getWindowVerticalPosition();
  uint8_t get2BPPPixel(uint8_t byte1, uint8_t byte2, int position);
  uint16_t get2BPPPixelRow(uint8_t byte1, uint8_t byte2);
};

}  // namespace GB

#endif  // INCLUDE_GB_PPU_HPP_
