#ifndef INCLUDE_GB_MMU_HPP_
#define INCLUDE_GB_MMU_HPP_

#include <array>
#include <cstdint>
#include <vector>

namespace GB {
class MMU {
 public:
  MMU();

  void initialize();
  void loadROM(const char* file_path);
  void write(uint16_t address, uint8_t data);
  uint8_t read(uint16_t address);
  void clearMemory();

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

  void setWindowXRegister(uint8_t data);
  uint8_t getWindowXRegister();
  void setWindowYRegister(uint8_t data);
  uint8_t getWindowYRegister();

  uint16_t get8000MethodAddress(uint8_t tile_number);
  uint16_t get8800MethodAddress(uint8_t tile_number);

 private:
  static const int address_space = 0x10000;
  std::array<uint8_t, address_space> memory;
  std::vector<uint8_t> read_only_memory;

  static const uint16_t DIV_addr = 0xFF04;
  static const uint16_t TIMA_addr = 0xFF05;
  static const uint16_t TMA_addr = 0xFF06;
  static const uint16_t TAC_addr = 0xFF07;
  static const uint16_t IE_addr = 0xFFFF;
  static const uint16_t IF_addr = 0xFF0F;
  static const uint16_t WX_addr = 0xFF4B;
  static const uint16_t WY_addr = 0xFF4A;

  bool checkBit(uint8_t n, uint8_t value) {
    if ((value >> n) & 0x01) {
      return true;
    } else {
      return false;
    }
  }

  uint8_t resetBit(uint8_t n, uint8_t value) { return value & ~(1 << n); }
};

}  // namespace GB

#endif  // INCLUDE_GB_MMU_HPP_
