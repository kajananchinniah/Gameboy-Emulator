#ifndef INCLUDE_GB_MMU_HPP_
#define INCLUDE_GB_MMU_HPP_

#include <array>
#include <cstdint>
#include <vector>

namespace GB {
class MMU {
 public:
  MMU();

  // MMU
  void initialize();
  void loadROM(const char* file_path);
  void write(uint16_t address, uint8_t data);
  uint8_t read(uint16_t address);
  void clearMemory();

  // Timer
  void incrementDividerRegister(uint8_t amount);
  void setDividerRegister(uint8_t data);
  uint8_t getDividerRegister();

  bool isTimerEnabled();
  uint8_t getInputClockSelect();

  void incrementTimerCounterRegister(uint8_t amount);
  void resetTimerCounterRegister();
  uint8_t getTimerCounterRegister();
  bool willTimerCounterRegisterOverflow();
  void setTimerInterrupt();

  // Interrupts
  bool isAnyInterruptEnabled();
  bool isAnyInterruptRequested();
  bool isVBlankInterruptEnabled();
  bool isVBlankInterruptRequested();
  bool isLCDStatInterruptEnabled();
  bool isLCDStatInterruptRequested();
  bool isTimerInterruptEnabled();
  bool isTimerInterruptRequested();
  bool isSerialInterruptEnabled();
  bool isSerialInterruptRequested();
  bool isJoypadInterruptEnabled();
  bool isJoypadInterruptRequested();

  void resetVBlankInterruptRequest();
  void resetLCDStatInterruptRequest();
  void resetTimerInterruptRequest();
  void resetSerialInterruptRequest();
  void resetJoypadInterruptRequest();

  // PPU
  void setWindowXRegister(uint8_t data);
  uint8_t getWindowXRegister();
  void setWindowYRegister(uint8_t data);
  uint8_t getWindowYRegister();

  void setLCDDisplayEnable();
  void resetLCDDisplayEnable();
  bool isLCDDisplayEnabled();

  void setWindowTileMapSelect();
  void resetWindowTileMapSelect();
  bool isWindowTileMapSelectSet();

  void setWindowDisplayEnable();
  void resetWindowDisplayEnable();
  bool isWindowDisplayEnabled();

  void setTileDataSelect();
  void resetTileDataSelect();
  bool isTileDataSelectSet();

  void setBGTileMapSelect();
  void resetBGTileMapSelect();
  bool isBGTileMapSelectSet();

  void setTallSpriteSize();
  void resetTallSpriteSize();
  bool isTallSpriteSizeSet();

  void setSpriteEnable();
  void resetSpriteEnable();
  bool isSpritesEnabled();

  void setBGAndWindowEnable();
  void resetBGAndWindowEnable();
  bool isBGAndWindowEnabled();

  void setLYCEqualLYSTATInterruptEnable();
  void resetLYCEqualLYSTATInterruptEnable();
  bool isLYCEqualLYSTATInterruptEnabled();

  void setMode2STATInterruptEnable();
  void resetMode2STATInterruptEnable();
  bool isMode2STATInterruptEnabled();

  void setMode1STATInterruptEnable();
  void resetMode1STATInterruptEnable();
  bool isMode1STATInterruptEnabled();

  void setMode0STATInterruptEnable();
  void resetMode0STATInterruptEnable();
  bool isMode0STATInterruptEnabled();

  void setCoincidenceFlag();
  void resetCoincidenceFlag();
  bool isCoincidenceFlagSet();

  void setPPUMode(uint8_t mode);
  uint8_t getPPUMode();

  uint16_t getPPU8000MethodAddress(uint8_t tile_number);
  uint16_t getPPU8800MethodAddress(uint8_t tile_number);

  uint32_t getOAMSpriteEntry(uint16_t offset);
  void setCurrentScanLine(uint8_t scanline);
  uint8_t getCurrentScanLine();

 private:
  static const int address_space = 0x10000;
  std::array<uint8_t, address_space> memory;
  std::vector<uint8_t> read_only_memory;

  // Timer registesr
  static const uint16_t DIV_addr = 0xFF04;
  static const uint16_t TIMA_addr = 0xFF05;
  static const uint16_t TMA_addr = 0xFF06;
  static const uint16_t TAC_addr = 0xFF07;

  // Interrupt registers
  static const uint16_t IE_addr = 0xFFFF;
  static const uint16_t IF_addr = 0xFF0F;

  // PPU registers
  static const uint16_t SCY_addr = 0xFF42;
  static const uint16_t SCX_addr = 0xFF43;
  static const uint16_t LY_addr = 0xFF44;
  static const uint16_t LYC_addr = 0xFF45;
  static const uint16_t WX_addr = 0xFF4B;
  static const uint16_t WY_addr = 0xFF4A;
  static const uint16_t LCDC_addr = 0xFF40;
  static const uint16_t STAT_addr = 0xFF41;
  static const uint16_t OAM_addr = 0xFE00;

  bool checkBit(uint8_t n, uint8_t value) {
    if ((value >> n) & 0x01) {
      return true;
    } else {
      return false;
    }
  }

  uint8_t setBit(uint8_t n, uint8_t value) { return value | (1 << n); }
  uint8_t resetBit(uint8_t n, uint8_t value) { return value & ~(1 << n); }
};

}  // namespace GB

#endif  // INCLUDE_GB_MMU_HPP_
