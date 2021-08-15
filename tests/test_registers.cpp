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
  EXPECT_EQ(registers.getA(), test_value >> 8);
  EXPECT_EQ(registers.getF(), test_value & 0x00ff);
}

TEST(RegisterBC, RandomNumber) {
  Registers registers;

  uint16_t test_value = generateRandomTestNumber<uint16_t>(0, 65535);
  registers.setBC(test_value);

  EXPECT_EQ(registers.getBC(), test_value);
  EXPECT_EQ(registers.getB(), test_value >> 8);
  EXPECT_EQ(registers.getC(), test_value & 0x00ff);
}

TEST(RegisterDE, RandomNumber) {
  Registers registers;

  uint16_t test_value = generateRandomTestNumber<uint16_t>(0, 65535);
  registers.setDE(test_value);

  EXPECT_EQ(registers.getDE(), test_value);
  EXPECT_EQ(registers.getD(), test_value >> 8);
  EXPECT_EQ(registers.getE(), test_value & 0x00ff);
}

TEST(RegisterHL, RandomNumber) {
  Registers registers;

  uint16_t test_value = generateRandomTestNumber<uint16_t>(0, 65535);
  registers.setHL(test_value);

  EXPECT_EQ(registers.getHL(), test_value);
  EXPECT_EQ(registers.getH(), test_value >> 8);
  EXPECT_EQ(registers.getL(), test_value & 0x00ff);
}

TEST(ZeroFlag, AllOp) {
  Registers registers;
  registers.setZeroFlag();
  EXPECT_EQ(registers.getZeroFlag(), 1);
  EXPECT_EQ(registers.getF(), 0b10000000);
  registers.clearZeroFlag();
  EXPECT_EQ(registers.getZeroFlag(), 0);
  EXPECT_EQ(registers.getF(), 0);
}

TEST(SubtractionFlag, AllOp) {
  Registers registers;
  registers.setSubtractionFlag();
  EXPECT_EQ(registers.getSubtractionFlag(), 1);
  EXPECT_EQ(registers.getF(), 0b01000000);
  registers.clearSubtractionFlag();
  EXPECT_EQ(registers.getSubtractionFlag(), 0);
  EXPECT_EQ(registers.getF(), 0);
}

TEST(HalfCarryFlag, AllOp) {
  Registers registers;
  registers.setHalfCarryFlag();
  EXPECT_EQ(registers.getHalfCarryFlag(), 1);
  EXPECT_EQ(registers.getF(), 0b00100000);
  registers.clearHalfCarryFlag();
  EXPECT_EQ(registers.getHalfCarryFlag(), 0);
  EXPECT_EQ(registers.getF(), 0);
}

TEST(CarryFlag, AllOp) {
  Registers registers;
  registers.setCarryFlag();
  EXPECT_EQ(registers.getCarryFlag(), 1);
  EXPECT_EQ(registers.getF(), 0b00010000);
  registers.clearCarryFlag();
  EXPECT_EQ(registers.getCarryFlag(), 0);
  EXPECT_EQ(registers.getF(), 0);
}

TEST(PC, RandomNumber) {
  Registers registers;
  uint16_t test_value = 65535;
  uint16_t test_increment = 65535;

  // Chosen values should not cause overflow
  while (static_cast<uint32_t>(test_value + test_increment) > 65535) {
    test_value = generateRandomTestNumber<uint16_t>(0, 65535);
    test_increment = generateRandomTestNumber<uint16_t>(0, 65535);
  }

  registers.setPC(test_value);
  EXPECT_EQ(registers.getPC(), test_value);
  registers.incrementPC(test_increment);
  EXPECT_EQ(registers.getPC(), test_value + test_increment);
  registers.decrementPC(test_increment);
  EXPECT_EQ(registers.getPC(), test_value);
}

TEST(SP, RandomNumber) {
  Registers registers;
  uint16_t test_value = 65535;
  uint16_t test_increment = 65535;

  // Chosen values should not cause overflow
  while (static_cast<uint32_t>(test_value + test_increment) > 65535) {
    test_value = generateRandomTestNumber<uint16_t>(0, 65535);
    test_increment = generateRandomTestNumber<uint16_t>(0, 65535);
  }

  registers.setSP(test_value);
  EXPECT_EQ(registers.getSP(), test_value);
  registers.incrementSP(test_increment);
  EXPECT_EQ(registers.getSP(), test_value + test_increment);
  registers.decrementSP(test_increment);
  EXPECT_EQ(registers.getSP(), test_value);
}

}  // namespace GB

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
