#include <gtest/gtest.h>

#include <cstdint>

#include "GB/MMU.hpp"
#include "GB/Opcodes_Load_base.hpp"
#include "GB/Registers.hpp"
#include "test_base.hpp"

namespace GB {

TEST(LD_ValueIntoRegister_8bit, RandomNumber) {
  Register random_register;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  OpcodesLoadBase::LD_ValueIntoRegister_8bit(random_register.getLowRegister(),
                                             &test_value);
  EXPECT_EQ(random_register.getLowValue(), test_value);
}

TEST(LD_MemoryLocationIntoRegister_8bit, RandomNumber) {
  Register random_register;
  MMU random_mmu;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  uint16_t test_addr = generateRandomTestNumber<uint16_t>(0, 65535);
  random_mmu.write(test_addr, test_value);
  OpcodesLoadBase::LD_MemoryLocationIntoRegister_8bit(
      random_register.getLowRegister(), &test_addr, &random_mmu);
  EXPECT_EQ(random_register.getLowValue(), random_mmu.read(test_addr));
}

TEST(LD_RegisterIntoMemoryLocation_8bit, RandomNumber) {
  Register random_register;
  MMU random_mmu;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  uint16_t test_addr = generateRandomTestNumber<uint16_t>(0, 65535);
  random_register.setLowValue(test_value);
  OpcodesLoadBase::LD_RegisterIntoMemoryLocation_8bit(
      &test_addr, random_register.getLowRegister(), &random_mmu);
  EXPECT_EQ(random_mmu.read(test_addr), random_register.getLowValue());
}

TEST(LDI_RegisterIntoMemoryLocation_8bit, RandomNumber) {
  Register random_register;
  MMU random_mmu;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  uint16_t test_addr = generateRandomTestNumber<uint16_t>(0, 65534);
  uint16_t copy_addr = test_addr;
  random_register.setLowValue(test_value);
  OpcodesLoadBase::LDI_RegisterIntoMemoryLocation_8bit(
      &test_addr, random_register.getLowRegister(), &random_mmu);
  EXPECT_EQ(random_mmu.read(copy_addr), random_register.getLowValue());
  EXPECT_EQ(test_addr - 1, copy_addr);
}

TEST(LDI_MemoryLocationIntoRegister_8bit, RandomNumber) {
  Register random_register;
  MMU random_mmu;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  uint16_t test_addr = generateRandomTestNumber<uint16_t>(0, 65534);
  uint16_t copy_addr = test_addr;
  random_mmu.write(test_addr, test_value);
  OpcodesLoadBase::LDI_MemoryLocationIntoRegister_8bit(
      random_register.getLowRegister(), &test_addr, &random_mmu);
  EXPECT_EQ(random_register.getLowValue(), random_mmu.read(copy_addr));
  EXPECT_EQ(test_addr - 1, copy_addr);
}

TEST(LDD_RegisterIntoMemoryLocation_8bit, RandomNumber) {
  Register random_register;
  MMU random_mmu;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  uint16_t test_addr = generateRandomTestNumber<uint16_t>(1, 65535);
  uint16_t copy_addr = test_addr;
  random_register.setLowValue(test_value);
  OpcodesLoadBase::LDD_RegisterIntoMemoryLocation_8bit(
      &test_addr, random_register.getLowRegister(), &random_mmu);
  EXPECT_EQ(random_mmu.read(copy_addr), random_register.getLowValue());
  EXPECT_EQ(test_addr + 1, copy_addr);
}

TEST(LDD_MemoryLocationIntoRegister_8bit, RandomNumber) {
  Register random_register;
  MMU random_mmu;
  uint8_t test_value = generateRandomTestNumber<uint8_t>(0, 255);
  uint16_t test_addr = generateRandomTestNumber<uint16_t>(1, 65535);
  uint16_t copy_addr = test_addr;
  random_mmu.write(test_addr, test_value);
  OpcodesLoadBase::LDD_MemoryLocationIntoRegister_8bit(
      random_register.getLowRegister(), &test_addr, &random_mmu);
  EXPECT_EQ(random_register.getLowValue(), random_mmu.read(copy_addr));
  EXPECT_EQ(test_addr + 1, copy_addr);
}

TEST(LD_ValueIntoRegister_16bit, RandomNumber) {
  Register random_register;
  uint16_t test_value = generateRandomTestNumber<uint16_t>(0, 65535);
  OpcodesLoadBase::LD_ValueIntoRegister_16bit(random_register.getFullRegister(),
                                              &test_value);
  EXPECT_EQ(random_register.getFullValue(), test_value);
}

TEST(LD_RegisterIntoMemoryLocation_16bit, RandomNumber) {
  Register random_register;
  MMU random_mmu;
  uint16_t test_addr = generateRandomTestNumber<uint16_t>(0, 65535);
  uint16_t test_value = generateRandomTestNumber<uint16_t>(0, 65535);
  random_register.setFullValue(test_value);
  OpcodesLoadBase::LD_RegisterIntoMemoryLocation_16bit(
      &test_addr, random_register.getFullRegister(), &random_mmu);
  EXPECT_EQ(random_mmu.read(test_addr & 0x00FF), random_register.getLowValue());
  EXPECT_EQ(random_mmu.read(test_addr >> 8), random_register.getHighValue());
}

}  // namespace GB

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
