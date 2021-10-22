#ifndef INCLUDE_GB_PPU_HPP_
#define INCLUDE_GB_PPU_HPP_

#include "MMU.hpp"

namespace GB {
class PPU {
 public:
  explicit PPU(MMU *mmu);
  ~PPU();
  void updatePPU(int clock_cycles);

 private:
  struct Colour {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  };

  static const unsigned int lcd_viewport_width{160};
  static const unsigned int lcd_viewport_height{144};
  static const unsigned int background_width{256};
  static const unsigned int background_height{256};
  static const unsigned int window_width{256};
  static const unsigned int window_height{256};
  static const unsigned int end_of_scanline_cycles{456};
  static const unsigned int max_scanline{153};
  unsigned int ppu_clock_cycles{0};

  // TODO: use STL array (I found C style array more readible here)
  uint8_t display_buffer[lcd_viewport_width][lcd_viewport_height][3];
  MMU *mmu;

  void resetPPU();
  void updatePPULCD();
  void doOAMScanMode();
  void doDrawingMode();
  void doHBlankMode();
  void doVBlankMode();
  void updateCoincidenceFlag();
  void prepareForNextScanLine();
  void renderScanLine();
  void renderTiles();
  uint8_t getBGAndWindowYPosition();
  uint16_t getBGAndWindowTileRow(uint8_t y_position);
  uint8_t getBGAndWindowXPosition(uint8_t pixel);
  uint16_t getBGAndWindowTileColumn(uint8_t x_position);
  uint8_t getCurrentVerticalBGAndVerticalLine(uint8_t y_position);
  uint8_t getColourPosition(uint8_t x_position);
  Colour decodeColour(uint8_t colour_id, uint16_t memory_addr);

  bool shouldUseWindow();
  bool shouldUse8000Mode();
  uint16_t getBackgroundMemoryAddress();
  void renderSprites();
  uint8_t getYPositionFromOAM(uint32_t entry);
  uint8_t getXPositionFromOAM(uint32_t entry);
  uint8_t getTileNumberFromOAM(uint32_t entry);
  uint8_t getSpriteFlagsFromOAM(uint32_t entry);
  bool isValidSpriteOAMEntry(uint8_t y, uint8_t x, uint8_t tile_num,
                             uint8_t sprite_flags);
  void addSpriteToDisplayBuffer(uint8_t y, uint8_t x, uint8_t tile_num,
                                uint8_t sprite_flags);

  uint8_t getWindowHorizontalPosition();
  uint8_t getWindowVerticalPosition();
  uint8_t get2BPPPixel(uint8_t byte1, uint8_t byte2, int position);
  uint16_t get2BPPPixelRow(uint8_t byte1, uint8_t byte2);
};

}  // namespace GB

#endif  // INCLUDE_GB_PPU_HPP_
