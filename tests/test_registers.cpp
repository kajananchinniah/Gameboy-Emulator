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

TEST(Register, RandomNumberHighSetter) {
  Register random_register;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  random_register.setHighValue(test_value);

  EXPECT_EQ(random_register.getHighValue(), test_value);
  EXPECT_EQ(random_register.getLowValue(), 0x00);

  EXPECT_EQ(*(random_register.getHighRegister()), test_value);
  EXPECT_EQ(*(random_register.getLowRegister()), 0x00);

  uint8_t extracted_value_high = random_register.getFullValue() >> 8;
  uint8_t extracted_value_low = random_register.getFullValue() & 0x00FF;
  EXPECT_EQ(extracted_value_high, test_value);
  EXPECT_EQ(extracted_value_low, 0x00);

  extracted_value_high = *(random_register.getFullRegister()) >> 8;
  extracted_value_low = *(random_register.getFullRegister()) & 0x00FF;
  EXPECT_EQ(extracted_value_high, test_value);
  EXPECT_EQ(extracted_value_low, 0x00);
}

TEST(Register, RandomNumberHighDirectWrite) {
  Register random_register;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  uint8_t *high = random_register.getHighRegister();
  *high = test_value;

  EXPECT_EQ(random_register.getHighValue(), test_value);
  EXPECT_EQ(random_register.getLowValue(), 0x00);

  EXPECT_EQ(*(random_register.getHighRegister()), test_value);
  EXPECT_EQ(*(random_register.getLowRegister()), 0x00);

  uint8_t extracted_value_high = random_register.getFullValue() >> 8;
  uint8_t extracted_value_low = random_register.getFullValue() & 0x00FF;
  EXPECT_EQ(extracted_value_high, test_value);
  EXPECT_EQ(extracted_value_low, 0x00);

  extracted_value_high = *(random_register.getFullRegister()) >> 8;
  EXPECT_EQ(extracted_value_high, test_value);
  EXPECT_EQ(extracted_value_low, 0x00);
}

TEST(Register, RandomNumberLowSetter) {
  Register random_register;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  random_register.setLowValue(test_value);

  EXPECT_EQ(random_register.getHighValue(), 0x00);
  EXPECT_EQ(random_register.getLowValue(), test_value);

  EXPECT_EQ(*(random_register.getHighRegister()), 0x00);
  EXPECT_EQ(*(random_register.getLowRegister()), test_value);

  uint8_t extracted_value_high = random_register.getFullValue() >> 8;
  uint8_t extracted_value_low = random_register.getFullValue() & 0x00FF;
  EXPECT_EQ(extracted_value_high, 0x00);
  EXPECT_EQ(extracted_value_low, test_value);

  extracted_value_high = *(random_register.getFullRegister()) >> 8;
  extracted_value_low = *(random_register.getFullRegister()) & 0x00FF;
  EXPECT_EQ(extracted_value_high, 0x00);
  EXPECT_EQ(extracted_value_low, test_value);
}

TEST(Register, RandomNumberLowDirectWrite) {
  Register random_register;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  uint8_t *low = random_register.getLowRegister();
  *low = test_value;

  EXPECT_EQ(random_register.getHighValue(), 0x00);
  EXPECT_EQ(random_register.getLowValue(), test_value);

  EXPECT_EQ(*(random_register.getHighRegister()), 0x00);
  EXPECT_EQ(*(random_register.getLowRegister()), test_value);

  uint8_t extracted_value_high = random_register.getFullValue() >> 8;
  uint8_t extracted_value_low = random_register.getFullValue() & 0x00FF;
  EXPECT_EQ(extracted_value_high, 0x00);
  EXPECT_EQ(extracted_value_low, test_value);

  extracted_value_high = *(random_register.getFullRegister()) >> 8;
  extracted_value_low = *(random_register.getFullRegister()) & 0x00FF;
  EXPECT_EQ(extracted_value_high, 0x00);
  EXPECT_EQ(extracted_value_low, test_value);
}

TEST(Register, RandomNumberFullSetter) {
  Register random_register;
  uint16_t test_value = generateRandomTestNumber<uint16_t>(0, 65535);
  uint8_t test_high_value = test_value >> 8;
  uint8_t test_low_value = test_value & 0x00FF;

  random_register.setFullValue(test_value);

  EXPECT_EQ(random_register.getHighValue(), test_high_value);
  EXPECT_EQ(random_register.getLowValue(), test_low_value);

  EXPECT_EQ(*(random_register.getHighRegister()), test_high_value);
  EXPECT_EQ(*(random_register.getLowRegister()), test_low_value);

  EXPECT_EQ(random_register.getFullValue(), test_value);
  EXPECT_EQ(*(random_register.getFullRegister()), test_value);
}

TEST(Register, RandomNumberFullDirectWrite) {
  Register random_register;

  uint16_t test_value = generateRandomTestNumber<uint16_t>(0, 65535);
  uint8_t test_high_value = test_value >> 8;
  uint8_t test_low_value = test_value & 0x00FF;

  uint16_t *full = random_register.getFullRegister();
  *full = test_value;

  EXPECT_EQ(random_register.getHighValue(), test_high_value);
  EXPECT_EQ(random_register.getLowValue(), test_low_value);

  EXPECT_EQ(*(random_register.getHighRegister()), test_high_value);
  EXPECT_EQ(*(random_register.getLowRegister()), test_low_value);

  EXPECT_EQ(random_register.getFullValue(), test_value);
  EXPECT_EQ(*(random_register.getFullRegister()), test_value);
}

TEST(ZeroFlag, AllOp) {
  RegisterAF AF;
  AF.setZeroFlag();
  EXPECT_EQ(AF.getZeroFlag(), 1);
  EXPECT_EQ(AF.getLowValue(), 0b10000000);
  EXPECT_EQ(*(AF.getLowRegister()), 0b10000000);
  AF.clearZeroFlag();
  EXPECT_EQ(AF.getZeroFlag(), 0);
  EXPECT_EQ(AF.getLowValue(), 0);
  EXPECT_EQ(*(AF.getLowRegister()), 0);
}

/*
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
*/

}  // namespace GB

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
