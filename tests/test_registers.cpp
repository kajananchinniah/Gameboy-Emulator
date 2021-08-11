#include <gtest/gtest.h>

#include <cstdint>
#include <random>

#include "GB/Registers.hpp"

namespace GB {
TEST(RegisterA, RandomNumber) {
  Registers registers;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, 255);

  uint8_t test_value = dist(mt);
  registers.setA(test_value);

  EXPECT_EQ(registers.getA(), test_value);

  uint8_t extracted_value = registers.getAF() >> 8;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterB, RandomNumber) {
  Registers registers;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, 255);

  uint8_t test_value = dist(mt);
  registers.setB(test_value);

  EXPECT_EQ(registers.getB(), test_value);

  uint8_t extracted_value = registers.getBC() >> 8;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterC, RandomNumber) {
  Registers registers;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, 255);

  uint8_t test_value = dist(mt);
  registers.setC(test_value);

  EXPECT_EQ(registers.getC(), test_value);

  uint8_t extracted_value = registers.getBC() & 0x00ff;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterD, RandomNumber) {
  Registers registers;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, 255);

  uint8_t test_value = dist(mt);
  registers.setD(test_value);

  EXPECT_EQ(registers.getD(), test_value);

  uint8_t extracted_value = registers.getDE() & 0x00ff;
  EXPECT_EQ(extracted_value, test_value);
}
}  // namespace GB

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
