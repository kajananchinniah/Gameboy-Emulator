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
  void printStatus();

  MMU mmu;
  RegisterAF AF;
  Register BC;
  Register DE;
  Register HL;

  ProgramCounter PC;
  StackPointer SP;

  int executeOpcode(uint8_t opcode);
  // X = arbitrary number (0 - F)
  int execute0x0XTable(uint8_t opcode);
  int execute0x1XTable(uint8_t opcode);
  int execute0x2XTable(uint8_t opcode);
  int execute0x3XTable(uint8_t opcode);
  int execute0x4XTable(uint8_t opcode);
  int execute0x5XTable(uint8_t opcode);
  int execute0x6XTable(uint8_t opcode);
  int execute0x7XTable(uint8_t opcode);
  int execute0x8XTable(uint8_t opcode);
  int execute0x9XTable(uint8_t opcode);
  int execute0xAXTable(uint8_t opcode);
  int execute0xBXTable(uint8_t opcode);
  int execute0xCXTable(uint8_t opcode);
  int execute0xDXTable(uint8_t opcode);
  int execute0xEXTable(uint8_t opcode);
  int execute0xFXTable(uint8_t opcode);

  int execute0xCBOpcode(uint8_t opcode);
  int execute0xCB0XTable(uint8_t opcode);
  int execute0xCB1XTable(uint8_t opcode);
  int execute0xCB2XTable(uint8_t opcode);
  int execute0xCB3XTable(uint8_t opcode);
  int execute0xCB4XTable(uint8_t opcode);
  int execute0xCB5XTable(uint8_t opcode);
  int execute0xCB6XTable(uint8_t opcode);
  int execute0xCB7XTable(uint8_t opcode);
  int execute0xCB8XTable(uint8_t opcode);
  int execute0xCB9XTable(uint8_t opcode);
  int execute0xCBAXTable(uint8_t opcode);
  int execute0xCBBXTable(uint8_t opcode);
  int execute0xCBCXTable(uint8_t opcode);
  int execute0xCBDXTable(uint8_t opcode);
  int execute0xCBEXTable(uint8_t opcode);
  int execute0xCBFXTable(uint8_t opcode);

  int unsupportedOpcode(uint8_t opcode);

  // Special instructions
  int NOP();

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
  int ld_A_FFC();
  int ld_FFC_A();
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
  int and_A_r(uint8_t const *r);
  int and_A_n();
  int and_A_HL();
  int xor_A_r(uint8_t const *r);
  int xor_A_n();
  int xor_A_HL();
  int or_A_r(uint8_t const *r);
  int or_A_n();
  int or_A_HL();
  int cp_A_r(uint8_t const *r);
  int cp_A_n();
  int cp_A_HL();
  int inc_r(uint8_t *r);
  int inc_HL();
  int dec_r(uint8_t *r);
  int daa_A();
  int cpl_A();

  // 16 bit arthimetic instructions
  int add_HL_rr(uint16_t const *rr);
  int inc_rr(uint16_t *rr);
  int dec_rr(uint16_t *rr);
  int add_SP_dd_relative();
  int ld_HL_SP_dd_relative();

  // Rotate and shift instructions
  int rlca_A();
  int rla_A();
  int rrca_A();
  int rra_A();
  int rlc_r(uint8_t *r);
  int rlc_HL();
  int rl_r(uint8_t *r);
  int rl_HL();
  int rrc_r(uint8_t *r);
  int rrc_HL();
  int rr_r(uint8_t *r);
  int rr_HL();
  int sla_r(uint8_t *r);
  int sla_HL();
  int swap_r(uint8_t *r);
  int swap_HL();
  int sra_r(uint8_t *r);
  int sra_HL();
  int srl_r(uint8_t *r);
  int srl_HL();

  // Single bit operation instructions

  // CPU control instructions
  int ccf();
  int scf();
  int nop();
  int halt();
  int stop();
  int di();
  int ei();

  // Jump instructions
  int jp_nn();
  int jp_HL();
  int jp_f_nn(uint8_t f, uint8_t condition);
  int jr_PC_dd();
  int jr_f_PC_dd(uint8_t f, uint8_t condition);
  int call_nn();
  int call_f_nn(uint8_t f, uint8_t condition);
  int ret();
  int ret_f(uint8_t f, uint8_t condition);
  int reti();
  int rst_n();
};
}  // namespace GB

#endif  // INCLUDE_GB_CPU_HPP_
