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

 private:
  static const int address_space = 0x10000;
  std::array<uint8_t, address_space> memory;
  std::vector<uint8_t> read_only_memory;
};

}  // namespace GB

#endif  // INCLUDE_GB_MMU_HPP_
