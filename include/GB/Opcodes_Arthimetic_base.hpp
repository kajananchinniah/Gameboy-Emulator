#ifndef INCLUDE_GB_OPCODES_ARTHIMETIC_BASE_HPP_
#define INCLUDE_GB_OPCODES_ARTHIMETIC_BASE_HPP_

#include <GB/MMU.hpp>
#include <cstdint>

namespace GB {
namespace OpcodesArthimeticBase {
void ADD_ValueIntoRegister_8bit(uint8_t *r, uint8_t const *n) { *r = *r + *n; }
void ADD_MemoryLocationIntoRegister_8bit(uint8_t *r, uint16_t const *addr,
                                         GB::MMU *mmu) {
  *r = *r + mmu->read(*addr);
}

void ADC_ValueIntoRegister_8bit(uint8_t *r, uint8_t const *n,
                                uint8_t const *carry) {
  *r = *r + *n + *carry;
}


}  // namespace OpcodesArthimeticBase

}  // namespace GB

#endif  // INCLUDE_GB_OPCODES_ARTHIMETIC_BASE_HPP_
