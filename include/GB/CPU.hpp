#ifndef INCLUDE_GB_CPU_HPP_
#define INCLUDE_GB_CPU_HPP_

#include <functional>
#include <vector>

#include "GB/MMU.hpp"
#include "GB/Registers.hpp"

namespace GB {
class CPU {
 public:
  CPU();
  void loadROM(const char *rom_path);
  void update();

 private:
  MMU mmu;
  RegisterAF AF;
  Register BC;
  Register DE;
  Register HL;

  ProgramCounter PC;
  StackPointer SP;

  unsigned int cycle();
  void executeNextOpcode();

  // 8 bit load instructions
  int ld_r_r(uint8_t *r1, uint8_t const *r2);
  int ld_r_n(uint8_t *r);
  int ld_r_HL(uint8_t *r);
  int ld_HL_r(uint8_t const *r);
  int ld_HL_n();
  int ld_A_BC();
  int ld_A_DE();
  int ld_BC_A();
  int ld_DE_A();
  int ld_A_nn();
  int ld_nn_A();
  int ld_A_C();
  int ld_C_A();
  int ld_A_n();
  int ld_n_A();
  int ldd_A_HL();
  int ldd_HL_A();
  int ldi_A_HL();
  int ldi_HL_A();

  // 16 bit load instructions
  int ld_rr_nn(uint16_t *rr);
  int ld_nn_SP();
  int ld_SP_HL();
  int push_rr(uint16_t *rr);
  int pop_rr(uint16_t *rr);

  // 8 bit arthimetic instructions
  int add_A_r(uint8_t const *r);
  int add_A_n();
  int add_A_HL();
  int adc_A_r(uint8_t const *r);
  int adc_A_n();
  int adc_A_HL();
  int sub_A_r(uint8_t const *r);
  int sub_A_n();
  int sub_A_HL();
  int sbc_A_r(uint8_t const *r);
  int sbc_A_n();
  int sbc_A_HL();
};
}  // namespace GB

#endif  // INCLUDE_GB_CPU_HPP_
