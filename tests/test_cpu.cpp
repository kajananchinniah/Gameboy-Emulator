#include <gtest/gtest.h>

#include "GB/CPU.hpp"

namespace GB {
TEST(CPU, test_06_ld_r_r_gb) {
  CPU cpu;
  std::string file_path(__FILE__);
  std::string dir_path = file_path.substr(0, file_path.rfind("/"));
  cpu.loadROM(
      (dir_path + std::string("/test_roms/cpu_instrs/individual/06-ld_r_r.gb"))
          .c_str());
  cpu.update();
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace GB
