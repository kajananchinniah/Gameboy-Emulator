#ifndef INCLUDE_GB_MMU_HPP_
#define INCLUDE_GB_MMU_HPP_

#include <array>
#include <cstdint>
#include <vector>

namespace GB {
class MMU {
 public:
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

 private:
  static const int address_space = 0x10000;
  std::array<uint8_t, address_space> memory;
  std::vector<uint8_t> read_only_memory;

  const uint16_t DIV_addr = 0xFF04;
  const uint16_t TIMA_addr = 0xFF05;
  const uint16_t TMA_addr = 0xFF06;
  const uint16_t TAC_addr = 0xFF07;
  const uint16_t IE_addr = 0xFFFF;
  const uint16_t IF_addr = 0xFF0F;
};

}  // namespace GB

#endif  // INCLUDE_GB_MMU_HPP_
