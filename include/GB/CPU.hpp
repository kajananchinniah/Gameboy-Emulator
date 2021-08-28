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
  /* void LD_r_n(uint8_t *r, const uint8_t n);
    void LD_r_addr(uint8_t *r, const uint16_t addr);
    void LD_addr_r(const uint16_t addr, const uint8_t r);
    void LDI_addr_r(uint16_t *addr, const uint8_t r);
    void LDI_r_addr(uint8_t *r, uint16_t *addr);
    void LDD_addr_r(uint16_t *addr, const uint8_t r);
    void LDD_r_addr(uint8_t *r, uint16_t *addr);

    // 16 bit load instructions
    void LD_rr_nn(uint16_t *rr, const uint16_t nn);
    void LD_addr_rr(const uint16_t addr, const uint16_t rr);
    void push_rr(const uint16_t rr);
    void pop_rr(const uint16_t rr); */
};
}  // namespace GB

#endif  // INCLUDE_GB_CPU_HPP_
