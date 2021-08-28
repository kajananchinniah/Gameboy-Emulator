#include "GB/CPU.hpp"

#include <iostream>

#include "GB/MMU.hpp"
#include "GB/Opcodes_Load_base.hpp"
namespace {}

namespace GB {
CPU::CPU() {
  // See: http://www.codeslinger.co.uk/pages/projects/gameboy/hardware.html
  AF.setFullValue(0x01B0);
  BC.setFullValue(0x0013);
  DE.setFullValue(0x00D8);
  HL.setFullValue(0x014D);
  PC.setPC(0x0100);
  SP.setSP(0xFFFE);

  mmu.write(0xFF05, 0x00);
  mmu.write(0xFF06, 0x00);
  mmu.write(0xFF07, 0x00);
  mmu.write(0xFF10, 0x80);
  mmu.write(0xFF11, 0xBF);
  mmu.write(0xFF12, 0xF3);
  mmu.write(0xFF14, 0xBF);
  mmu.write(0xFF16, 0x3F);
  mmu.write(0xFF17, 0x00);
  mmu.write(0xFF19, 0xBF);
  mmu.write(0xFF1A, 0x7F);
  mmu.write(0xFF1B, 0xFF);
  mmu.write(0xFF1C, 0x9F);
  mmu.write(0xFF1E, 0xBF);
  mmu.write(0xFF20, 0xFF);
  mmu.write(0xFF21, 0x00);
  mmu.write(0xFF22, 0x00);
  mmu.write(0xFF23, 0xBF);
  mmu.write(0xFF24, 0x77);
  mmu.write(0xFF25, 0xF3);
  mmu.write(0xFF26, 0xF1);
  mmu.write(0xFF40, 0x91);
  mmu.write(0xFF42, 0x00);
  mmu.write(0xFF43, 0x00);
  mmu.write(0xFF45, 0x00);
  mmu.write(0xFF47, 0xFC);
  mmu.write(0xFF48, 0xFF);
  mmu.write(0xFF49, 0xFF);
  mmu.write(0xFF4A, 0x00);
  mmu.write(0xFF4B, 0x00);
  mmu.write(0xFFFF, 0x00);
}

void CPU::loadROM(const char *rom_path) { mmu.loadROM(rom_path); }

// Opcodes

// 8 bit load instructions
int CPU::ld_r_r(uint8_t *r1, uint8_t const *r2) {
  *r1 = *r2;
  return 4;
}

int CPU::ld_r_n(uint8_t *r) {
  *r = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  return 8;
}

int CPU::ld_r_HL(uint8_t *r) {
  *r = mmu.read(HL.getFullValue());
  return 8;
}

int CPU::ld_HL_r(uint8_t const *r) {
  mmu.write(HL.getFullValue(), *r);
  return 8;
}

int CPU::ld_HL_n() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  mmu.write(HL.getFullValue(), n);
  return 12;
}

int CPU::ld_A_BC() {
  AF.setHighValue(mmu.read(BC.getFullValue()));
  return 8;
}

int CPU::ld_A_DE() {
  AF.setHighValue(mmu.read(DE.getFullValue()));
  return 8;
}

int CPU::ld_BC_A() {
  mmu.write(BC.getFullValue(), AF.getHighValue());
  return 8;
}

int CPU::ld_DE_A() {
  mmu.write(DE.getFullValue(), AF.getHighValue());
  return 8;
}

int CPU::ld_A_nn() {
  uint8_t low = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t high = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint16_t nn = high << 8 | low;
  AF.setHighValue(mmu.read(nn));
  return 16;
}

int CPU::ld_nn_A() {
  uint8_t low = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t high = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint16_t nn = high << 8 | low;
  mmu.write(nn, AF.getHighValue());
  return 16;
}

int CPU::ld_A_C() {
  uint8_t low = BC.getLowValue();
  uint8_t high = 0xFF;
  uint16_t addr = high << 8 | low;
  AF.setHighValue(mmu.read(addr));
  return 8;
}

int CPU::ld_C_A() {
  uint8_t low = BC.getLowValue();
  uint8_t high = 0xFF;
  uint16_t addr = high << 8 | low;
  mmu.write(addr, AF.getHighValue());
  return 8;
}

int CPU::ld_A_n() {
  uint8_t low = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t high = 0xFF;
  uint16_t addr = high << 8 | low;
  AF.setHighValue(mmu.read(addr));
  return 12;
}

int CPU::ld_n_A() {
  uint8_t low = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t high = 0xFF;
  uint16_t addr = high << 8 | low;
  mmu.write(addr, AF.getHighValue());
  return 12;
}

int CPU::ldd_A_HL() {
  AF.setHighValue(mmu.read(HL.getFullValue()));
  HL.setFullValue(HL.getFullValue() - 1);
  return 8;
}

int CPU::ldd_HL_A() {
  mmu.write(HL.getFullValue(), AF.getHighValue());
  HL.setFullValue(HL.getFullValue() - 1);
  return 8;
}

int CPU::ldi_A_HL() {
  AF.setHighValue(mmu.read(HL.getFullValue()));
  HL.setFullValue(HL.getFullValue() + 1);
  return 8;
}

int CPU::ldi_HL_A() {
  mmu.write(HL.getFullValue(), AF.getHighValue());
  HL.setFullValue(HL.getFullValue() + 1);
  return 8;
}

// 16 bit load instructions
int CPU::ld_rr_nn(uint16_t *rr) {
  uint8_t low = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t high = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint16_t nn = high << 8 | low;
  *rr = nn;
  return 12;
}

int CPU::ld_nn_SP() {
  uint8_t low = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t high = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint16_t nn = high << 8 | low;

  uint16_t SP_value = SP.getSPValue();
  uint8_t SP_low = SP_value & 0x00FF;
  uint8_t SP_high = SP_value >> 8;

  mmu.write(nn, SP_low);
  mmu.write(nn + 1, SP_high);
  return 20;
}

int CPU::ld_SP_HL() {
  SP.setSP(HL.getFullValue());
  return 8;
}

int CPU::push_rr(uint16_t *rr) {
  uint8_t low = *rr & 0x00FF;
  uint8_t high = *rr >> 8;

  SP.decrementSP(1);
  mmu.write(SP.getSPValue(), high);
  SP.decrementSP(1);
  mmu.write(SP.getSPValue(), low);
  return 16;
}

int CPU::pop_rr(uint16_t *rr) {
  uint8_t low = mmu.read(SP.getSPValue());
  SP.incrementSP(1);
  uint8_t high = mmu.read(SP.getSPValue());
  SP.incrementSP(1);
  *rr = high << 8 | low;
  return 12;
}

}  // namespace GB
