#ifndef INCLUDE_GB_PPU_HPP_
#define INCLUDE_GB_PPU_HPP_

#include "MMU.hpp"

namespace GB {
class PPU {
 public:
  PPU(MMU *mmu);
  ~PPU();

 private:
  const unsigned int lcd_viewport_width{160};
  const unsigned int lcd_viewport_height{144};
  const unsigned int background_width{256};
  const unsigned int background_height{256};
  const unsigned int window_width{256};
  const unsigned int window_height{256};
  const MMU *mmu;
};

}  // namespace GB

#endif  // INCLUDE_GB_PPU_HPP_
