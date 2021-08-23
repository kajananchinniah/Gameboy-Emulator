#ifndef INCLUDE_GB_OPCODES_BASE_HPP_
#define INCLUDE_GB_OPCODES_BASE_HPP_

#include <cstdint>
#include <GB/MMU.hpp>

namespace GB {
namespace OpcodesBase {

void LD_ValueIntoRegister_8bit(uint8_t *r, uint8_t const *n) { *r = *n; }

void LD_MemoryLocationIntoRegister_8bit(uint8_t *r, uint16_t const *addr,
                                        GB::MMU *mmu) {
  *r = mmu->read(*addr);
}

void LD_RegisterIntoMemoryLocation_8bit(uint16_t const *addr, uint8_t const *r,
                                        GB::MMU *mmu) {
  mmu->write(*addr, *r);
}

void LDI_RegisterIntoMemoryLocation_8bit(uint16_t *addr, uint8_t const *r,
                                         GB::MMU *mmu) {
  mmu->write(*addr, *r);
  *addr = *addr + 1;
}

void LDI_MemoryLocationIntoRegister_8bit(uint8_t *r, uint16_t *addr,
                                         GB::MMU *mmu) {
  *r = mmu->read(*addr);
  *addr = *addr + 1;
}

void LDD_RegisterIntoMemoryLocation_8bit(uint16_t *addr, uint8_t const *r,
                                         GB::MMU *mmu) {
  mmu->write(*addr, *r);
  *addr = *addr - 1;
}

void LDD_MemoryLocationIntoRegister_8bit(uint8_t *r, uint16_t *addr,
                                         GB::MMU *mmu) {
  *r = mmu->read(*addr);
  *addr = *addr - 1;
}

void LD_ValueIntoRegister_16bit(uint16_t *rr, uint16_t const *nn) { *rr = *nn; }
void LD_RegisterIntoMemoryLocation_16bit(uint16_t const *addr,
                                         uint16_t const *rr, GB::MMU *mmu) {
  uint8_t high_addr = (*addr) >> 8;
  uint8_t low_addr = (*addr) & 0x00FF;

  uint8_t high_reg = (*rr) >> 8;
  uint8_t low_reg = (*rr) & 0x00FF;

  mmu->write(high_addr, high_reg);
  mmu->write(low_addr, low_reg);
}

void Push_Register_16bit(uint16_t *SP, uint16_t *rr, GB::MMU *mmu) {
  uint8_t high_reg = (*rr) >> 8;
  uint8_t low_reg = (*rr) & 0x00FF;
  mmu->write(*SP - 1, high_reg);
  mmu->write(*SP - 2, low_reg);
  *SP = *SP - 2;
}

void Pop_Register_16bit(uint16_t *SP, uint16_t *rr, GB::MMU *mmu) {
  uint8_t low_reg = mmu->read(*SP);
  uint8_t high_reg = mmu->read(*SP + 1);
  *SP = *SP + 2;
  *rr = high_reg << 8 | low_reg;
}


}  // namespace OpcodesBase
}  // namespace GB

#endif  // INCLUDE_GB_OPCODES_BASE_HPP_
