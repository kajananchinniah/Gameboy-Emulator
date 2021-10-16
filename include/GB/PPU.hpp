#ifndef INCLUDE_GB_PPU_HPP_
#define INCLUDE_GB_PPU_HPP_

#include <queue>
#include <vector>

#include "MMU.hpp"

namespace GB {
class PPU {
 public:
  explicit PPU(MMU *mmu);
  ~PPU();
  void updatePPU(int clock_cycles);

 private:
  const unsigned int lcd_viewport_width{160};
  const unsigned int lcd_viewport_height{144};
  const unsigned int background_width{256};
  const unsigned int background_height{256};
  const unsigned int window_width{256};
  const unsigned int window_height{256};
  const unsigned int kOAMScanCycles{80};
  const unsigned int kDrawingCycles{252};
  const unsigned int kMaxPPUClockCycles{456};
  unsigned int ppu_clock_cycles;
  MMU *mmu;

  struct OAMEntry {
    uint8_t y_position;
    uint8_t x_position;
    uint8_t tile_number;
    uint8_t sprite_flags;
  };

  uint16_t oam_sprite_number{0};
  std::vector<OAMEntry> sprite_buffer;

  uint8_t getWindowHorizontalPosition();
  uint8_t getWindowVerticalPosition();
  uint8_t get2BPPPixel(uint8_t byte1, uint8_t byte2, int position);
  uint16_t get2BPPPixelRow(uint8_t byte1, uint8_t byte2);

  void updatePPULCD(int clock_cycleS);

  void performOAMScanMode(int clock_cycles);
  uint8_t getXPositionFromOAM(uint32_t entry);
  uint8_t getYPositionFromOAM(uint32_t entry);
  uint8_t getTileNumberFromOAM(uint32_t entry);
  uint8_t getSpriteFlagsFromOAM(uint32_t entry);
  bool isValidSpriteOAMEntry(uint8_t x, uint8_t y, uint8_t tile_number,
                             uint8_t sprite_flags);

  void resetPPU();
  void performVBlankMode(int clock_cycles);
  void performDrawingMode(int clock_cycleS);
  void performHBlankMode(int clock_cycles);
  void requestPPUInterrupt();

  std::queue<uint8_t> background_fifo;
  std::queue<uint8_t> sprite_fifo;
  void initPixelFIFO();
  int background_fifo_x_count{0};
};

}  // namespace GB

#endif  // INCLUDE_GB_PPU_HPP_
