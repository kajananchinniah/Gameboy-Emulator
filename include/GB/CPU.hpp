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

  // Jump instructions
  int jp_nn();
  int jp_HL();
};
}  // namespace GB

#endif  // INCLUDE_GB_CPU_HPP_
