#ifndef INCLUDE_GB_PPU_HPP_
#define INCLUDE_GB_PPU_HPP_

#include <optional>

#include "MMU.hpp"

namespace GB {

struct Colour {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  int32_t colour;
  Colour() : red{0}, green{0}, blue{0}, colour{0} {}
  Colour(uint8_t r, uint8_t g, uint8_t b, int32_t c)
      : red{r}, green{g}, blue{b}, colour{c} {}
};

class PPU {
 public:
  explicit PPU(MMU *mmu);
  ~PPU();
  void updatePPU(int clock_cycles);
  unsigned int getLCDViewportWidth() { return 160; }
  unsigned int getLCDViewportHeight() { return 144; }

  static const unsigned int lcd_viewport_width{160};
  static const unsigned int lcd_viewport_height{144};
  uint8_t display_buffer[lcd_viewport_width][lcd_viewport_height][3];

 private:
  static const unsigned int background_width{256};
  static const unsigned int background_height{256};
  static const unsigned int window_width{256};
  static const unsigned int window_height{256};
  static const unsigned int end_of_scanline_cycles{456};
  static const unsigned int max_scanline{153};
  unsigned int ppu_clock_cycles{0};

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
  uint8_t getBGAndWindowColourPosition(uint8_t x_position);
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
  uint8_t getSpriteVerticalLine(uint8_t y_position, uint8_t sprite_flags);
  uint16_t getSpriteDataAddress(uint8_t tile_number, uint8_t line);
  uint8_t getSpriteColourPosition(int8_t tile_pixel, uint8_t sprite_flags);
  uint16_t getSpriteColourAddress(uint8_t sprite_flags);
  int getSpritePixelLocation(uint8_t x_position, int8_t tile_pixel);

  uint8_t getWindowHorizontalPosition();
  uint8_t getWindowVerticalPosition();
  uint8_t get2BPPPixel(uint8_t byte1, uint8_t byte2, int position);
  uint16_t get2BPPPixelRow(uint8_t byte1, uint8_t byte2);

  uint8_t getBit(uint8_t n, uint8_t value) { return ((value >> n) & 0x01); }
  bool checkBit(uint8_t n, uint8_t value) {
    if ((value >> n) & 0x01) {
      return true;
    } else {
      return false;
    }
  }
};

}  // namespace GB

#endif  // INCLUDE_GB_PPU_HPP_
