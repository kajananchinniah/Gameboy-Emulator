#include "GB/CPU.hpp"

#include <iomanip>
#include <iostream>

#include "GB/MMU.hpp"
namespace {}

namespace GB {

void CPU::printStatus() {
  std::cout << "AF = 0x" << std::hex << AF.getFullValue() << "\n";
  std::cout << "BC = 0x" << std::hex << BC.getFullValue() << "\n";
  std::cout << "DE = 0x" << std::hex << DE.getFullValue() << "\n";
  std::cout << "HL = 0x" << std::hex << HL.getFullValue() << "\n";
  std::cout << "SP = 0x" << std::hex << SP.getSPValue() << "\n";
  std::cout << "PC = 0x" << std::hex << PC.getPCValue() - 1 << "\n";
}

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

void CPU::update() {
  while (true) {
    uint8_t opcode = mmu.read(PC.getPCValue());
    PC.incrementPC(1);
    executeOpcode(opcode);
    if (mmu.read(0xff02) == 0x81) {
      char c = mmu.read(0xff01);
      printf("%c", c);
      mmu.write(0xff02, 0x00);
      exit(0);
    }
  }
}

int CPU::unsupportedOpcode(uint8_t opcode) {
  std::stringstream error_stream;
  error_stream << "Unknown opcode: 0x" << std::hex << (int)opcode << " at 0x"
               << std::hex << (int)PC.getPCValue();
  throw std::runtime_error(error_stream.str());
  return -1;
}

int CPU::executeOpcode(uint8_t opcode) {
  std::cout << "Executing: 0x" << std::hex << (int)opcode << " at 0x"
            << std::hex << (int)PC.getPCValue() - 1 << "\n";
  printStatus();
  std::cout << "\n\n";

  switch (opcode & 0xF0) {
    case 0x00:
      return execute0x0XTable(opcode);
      break;
    case 0x10:
      return execute0x1XTable(opcode);
      break;
    case 0x20:
      return execute0x2XTable(opcode);
      break;
    case 0x30:
      return execute0x3XTable(opcode);
      break;
    case 0x40:
      return execute0x4XTable(opcode);
      break;
    case 0x50:
      return execute0x5XTable(opcode);
      break;
    case 0x60:
      return execute0x6XTable(opcode);
      break;
    case 0x70:
      return execute0x7XTable(opcode);
      break;
    case 0x80:
      return execute0x8XTable(opcode);
      break;
    case 0x90:
      return execute0x9XTable(opcode);
      break;
    case 0xA0:
      return execute0xAXTable(opcode);
      break;
    case 0xB0:
      return execute0xBXTable(opcode);
      break;
    case 0xC0:
      return execute0xCXTable(opcode);
      break;
    case 0xD0:
      return execute0xDXTable(opcode);
      break;
    case 0xE0:
      return execute0xEXTable(opcode);
      break;
    case 0xF0:
      return execute0xFXTable(opcode);
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}

int CPU::execute0x0XTable(uint8_t opcode) {
  switch (opcode & 0x0F) {
    case 0x00:
      return NOP();
      break;
    case 0x01:
      return ld_rr_nn(BC.getFullRegister());
      break;
    case 0x02:
      return ld_BC_A();
      break;
    case 0x03:
      return 0;
      break;
    case 0x0D:
      return dec_r(BC.getLowRegister());
      break;
    case 0x0E:
      return ld_r_n(BC.getLowRegister());
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}

int CPU::execute0x1XTable(uint8_t opcode) {
  switch (opcode & 0x0F) {
    case 0x01:
      return ld_rr_nn(DE.getFullRegister());
      break;
    case 0x02:
      return ld_DE_A();
      break;
    case 0x04:
      return inc_r(DE.getHighRegister());
      break;
    case 0x0c:
      return inc_r(DE.getLowRegister());
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}
int CPU::execute0x2XTable(uint8_t opcode) {
  switch (opcode & 0x0F) {
    case 0x00:
      return jr_f_PC_dd(AF.getZeroFlag(), 0);
      break;
    case 0x01:
      return ld_rr_nn(HL.getFullRegister());
      break;
    case 0x0A:
      return ldi_A_HL();
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}
int CPU::execute0x3XTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0x4XTable(uint8_t opcode) {
  switch (opcode & 0x0F) {
    case 0x07:
      return ld_r_r(BC.getHighRegister(), AF.getHighRegister());
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
  return 0;
}
int CPU::execute0x5XTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0x6XTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0x7XTable(uint8_t opcode) {
  switch (opcode & 0x0F) {
    case 0x08:
      return ld_r_r(AF.getHighRegister(), BC.getHighRegister());
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}
int CPU::execute0x8XTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0x9XTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0xAXTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0xBXTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0xCXTable(uint8_t opcode) {
  switch (opcode & 0x0F) {
    case 0x03:
      return jp_nn();
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}
int CPU::execute0xDXTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0xEXTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0xFXTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
// Opcodes

// Special instructions
int CPU::NOP() { return 1; }

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

int CPU::ld_A_FFC() {
  uint8_t low = BC.getLowValue();
  uint8_t high = 0xFF;
  uint16_t addr = high << 8 | low;
  AF.setHighValue(mmu.read(addr));
  return 8;
}

int CPU::ld_FFC_A() {
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

// 8 bit arthimetic operations
int CPU::inc_r(uint8_t *r) {
  uint8_t tmp = *r + 1;
  if (tmp == 0) {
    AF.setZeroFlag();
  } else {
    AF.clearZeroFlag();
  }

  AF.clearSubtractionFlag();

  if ((*r & 0x0F) == 0x0F) {
    AF.setHalfCarryFlag();
  } else {
    AF.clearHalfCarryFlag();
  }

  *r = tmp;
  return 4;
}

int CPU::dec_r(uint8_t *r) {
  uint8_t tmp = *r - 1;
  if (tmp == 0) {
    AF.setZeroFlag();
  } else {
    AF.clearZeroFlag();
  }

  AF.setSubtractionFlag();
  if ((*r & 0x0F) == 0x00) {
    AF.setHalfCarryFlag();
  } else {
    AF.clearHalfCarryFlag();
  }

  *r = tmp;
  return 4;
}

// Jump instructions
int CPU::jp_nn() {
  uint8_t low = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t high = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint16_t nn = high << 8 | low;
  PC.setPC(nn);
  return 16;
}

int CPU::jp_HL() {
  PC.setPC(HL.getFullValue());
  return 4;
}

int CPU::jr_f_PC_dd(uint8_t f, uint8_t condition) {
  int8_t dd = static_cast<int8_t>(mmu.read(PC.getPCValue()));
  PC.incrementPC(1);
  if (f == condition) {
    PC.incrementPC(dd);
    return 12;
  }
  return 8;
}
}  // namespace GB
