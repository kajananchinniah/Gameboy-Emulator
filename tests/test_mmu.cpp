#include <gtest/gtest.h>

#include <cstdint>

#include "GB/MMU.hpp"
#include "test_base.hpp"

namespace GB {
TEST(MMU, ReadAndWrite) {
  MMU random_mmu;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  uint16_t test_addr = generateRandomTestNumber<uint16_t>(0, 65535);
  random_mmu.write(test_addr, test_value);
  EXPECT_EQ(random_mmu.read(test_addr), test_value);
}

}  // namespace GB

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
