#include <gtest/gtest.h>

#include <cstdint>
#include <random>

#include "GB/Registers.hpp"

namespace GB {

template <typename T>
T generateRandomTestNumber(T min, T max) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<T> dist(min, max);
  return dist(mt);
}

TEST(RegisterA, RandomNumber) {
  Registers registers;

  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  registers.setA(test_value);

  EXPECT_EQ(registers.getA(), test_value);

  uint8_t extracted_value = registers.getAF() >> 8;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterB, RandomNumber) {
  Registers registers;

  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  registers.setB(test_value);

  EXPECT_EQ(registers.getB(), test_value);

  uint8_t extracted_value = registers.getBC() >> 8;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterC, RandomNumber) {
  Registers registers;

  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  registers.setC(test_value);

  EXPECT_EQ(registers.getC(), test_value);

  uint8_t extracted_value = registers.getBC() & 0x00ff;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterD, RandomNumber) {
  Registers registers;

  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  registers.setD(test_value);

  EXPECT_EQ(registers.getD(), test_value);

  uint8_t extracted_value = registers.getDE() >> 8;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterE, RandomNumber) {
  Registers registers;

  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  registers.setE(test_value);

  EXPECT_EQ(registers.getE(), test_value);

  uint8_t extracted_value = registers.getDE() & 0x00ff;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterF, RandomNumber) {
  Registers registers;

  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  registers.setF(test_value);

  EXPECT_EQ(registers.getF(), test_value);

  uint8_t extracted_value = registers.getAF() & 0x00ff;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterH, RandomNumber) {
  Registers registers;

  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  registers.setH(test_value);

  EXPECT_EQ(registers.getH(), test_value);

  uint8_t extracted_value = registers.getHL() >> 8;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterL, RandomNumber) {
  Registers registers;

  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  registers.setL(test_value);

  EXPECT_EQ(registers.getL(), test_value);

  uint8_t extracted_value = registers.getHL() & 0x00ff;
  EXPECT_EQ(extracted_value, test_value);
}

TEST(RegisterAF, RandomNumber) {
  Registers registers;

  uint16_t test_value = generateRandomTestNumber<uint16_t>(0, 65535);
  registers.setAF(test_value);

  EXPECT_EQ(registers.getAF(), test_value);

  uint8_t extracted_high = registers.getAF() >> 8;
  EXPECT_EQ(extracted_high, test_value >> 8);

  uint8_t extracted_low = registers.getAF() & 0x00ff;
  EXPECT_EQ(extracted_low, test_value & 0x00ff);
}

}  // namespace GB

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
