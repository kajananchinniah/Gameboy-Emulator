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
      return nop();
      break;
    case 0x01:
      return ld_rr_nn(BC.getFullRegister());
      break;
    case 0x02:
      return ld_BC_A();
      break;
    case 0x03:
      return inc_rr(BC.getFullRegister());
      break;
    case 0x04:
      return inc_r(BC.getHighRegister());
      break;
    case 0x05:
      return dec_r(BC.getHighRegister());
      break;
    case 0x06:
      return ld_r_n(BC.getHighRegister());
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
    case 0x08:
      return jr_PC_dd();
      break;
    case 0x0A:
      return ld_A_DE();
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
int CPU::execute0x3XTable(uint8_t opcode) {
  switch (opcode & 0x0F) {
    case 0x01:
      return ld_rr_nn(SP.getSP());
      break;
    case 0x0E:
      return ld_r_n(AF.getHighRegister());
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}
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
    case 0x07:
      return ld_HL_r(AF.getHighRegister());
      break;
    case 0x08:
      return ld_r_r(AF.getHighRegister(), BC.getHighRegister());
      break;
    case 0x09:
      return ld_r_r(AF.getHighRegister(), BC.getLowRegister());
      break;
    case 0x0A:
      return ld_r_r(AF.getHighRegister(), DE.getHighRegister());
      break;
    case 0x0B:
      return ld_r_r(AF.getHighRegister(), DE.getLowRegister());
      break;
    case 0x0C:
      return ld_r_r(AF.getHighRegister(), HL.getHighRegister());
      break;
    case 0x0D:
      return ld_r_r(AF.getHighRegister(), HL.getLowRegister());
      break;
    case 0x0E:
      return ld_r_HL(AF.getHighRegister());
      break;
    case 0x0F:
      return ld_r_r(AF.getHighRegister(), AF.getHighRegister());
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
    case 0x09:
      return ret();
      break;
    case 0x0D:
      return call_nn();
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}
int CPU::execute0xDXTable(uint8_t opcode) { return unsupportedOpcode(opcode); }
int CPU::execute0xEXTable(uint8_t opcode) {
  switch (opcode & 0x0F) {
    case 0x00:
      return ld_FFn_A();
      break;
    case 0x01:
      return pop_rr(HL.getFullRegister());
      break;
    case 0x05:
      return push_rr(HL.getFullRegister());
      break;
    case 0x0A:
      return ld_nn_A();
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}
int CPU::execute0xFXTable(uint8_t opcode) {
  switch (opcode & 0x0F) {
    case 0x01:
      return pop_rr(AF.getFullRegister());
      break;
    case 0x03:
      return di();
      break;
    case 0x05:
      return push_rr(AF.getFullRegister());
      break;
    default:
      return unsupportedOpcode(opcode);
      break;
  }
}
// Opcodes

// Special instructions

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

uint16_t get_nn(MMU *mmu, ProgramCounter *PC) {
  uint8_t low = mmu->read(PC->getPCValue());
  PC->incrementPC(1);
  uint8_t high = mmu->read(PC->getPCValue());
  PC->incrementPC(1);
  return high << 8 | low;
}

int CPU::ld_A_nn() {
  uint16_t nn = get_nn(&mmu, &PC);
  AF.setHighValue(mmu.read(nn));
  return 16;
}

int CPU::ld_nn_A() {
  uint16_t nn = get_nn(&mmu, &PC);
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

int CPU::ld_A_FFn() {
  uint8_t low = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t high = 0xFF;
  uint16_t addr = high << 8 | low;
  AF.setHighValue(mmu.read(addr));
  return 12;
}

int CPU::ld_FFn_A() {
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
  uint16_t nn = get_nn(&mmu, &PC);
  *rr = nn;
  return 12;
}

int CPU::ld_nn_SP() {
  uint16_t nn = get_nn(&mmu, &PC);

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

uint8_t add_8bit(uint8_t operand1, uint8_t operand2, RegisterAF *AF,
                 uint8_t carry, bool modify_carry) {
  uint8_t result = operand1 + operand2 + carry;
  if (result == 0) {
    AF->setZeroFlag();
  } else {
    AF->clearZeroFlag();
  }

  AF->clearSubtractionFlag();

  if (((operand1 & 0x0F) + (operand2 & 0x0F)) > 0x0F) {
    AF->setHalfCarryFlag();
  } else {
    AF->clearHalfCarryFlag();
  }

  if (!modify_carry) {
    return result;
  }

  if ((operand1 + operand2) > 0xFF) {
    AF->setCarryFlag();
  } else {
    AF->clearCarryFlag();
  }
  return result;
}

uint8_t sub_8bit(uint8_t operand1, uint8_t operand2, RegisterAF *AF,
                 uint8_t carry, bool modify_carry) {
  uint8_t result = operand1 - operand2 - carry;
  if (result == 0) {
    AF->setZeroFlag();
  } else {
    AF->clearZeroFlag();
  }

  AF->setSubtractionFlag();

  if ((operand1 & 0x0F) < (operand2 & 0x0F)) {
    AF->setHalfCarryFlag();
  } else {
    AF->clearHalfCarryFlag();
  }

  if (!modify_carry) {
    return result;
  }

  if (operand1 < operand2) {
    AF->setCarryFlag();
  } else {
    AF->clearCarryFlag();
  }
  return result;
}

// 8 bit arthimetic operations
int CPU::add_A_r(uint8_t const *r) {
  uint8_t result = add_8bit(AF.getHighValue(), *r, &AF, 0, true);
  AF.setHighValue(result);
  return 4;
}

int CPU::add_A_n() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);

  uint8_t result = add_8bit(AF.getHighValue(), n, &AF, 0, true);
  AF.setHighValue(result);
  return 8;
}

int CPU::add_A_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  uint8_t result = add_8bit(AF.getHighValue(), value, &AF, 0, true);
  AF.setHighValue(result);
  return 8;
}
int CPU::adc_A_r(uint8_t const *r) {
  uint8_t result =
      add_8bit(AF.getHighValue(), *r, &AF, AF.getCarryFlag(), true);
  AF.setHighValue(result);
  return 4;
}

int CPU::adc_A_n() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);

  uint8_t result = add_8bit(AF.getHighValue(), n, &AF, AF.getCarryFlag(), true);
  AF.setHighValue(result);
  return 8;
}

int CPU::adc_A_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  uint8_t result =
      add_8bit(AF.getHighValue(), value, &AF, AF.getCarryFlag(), true);
  AF.setHighValue(result);
  return 8;
}

int CPU::sub_A_r(uint8_t const *r) {
  uint8_t result = sub_8bit(AF.getHighValue(), *r, &AF, 0, true);
  AF.setHighValue(result);
  return 4;
}

int CPU::sub_A_n() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t result = sub_8bit(AF.getHighValue(), n, &AF, 0, true);
  AF.setHighValue(result);
  return 8;
}

int CPU::sub_A_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  uint8_t result = sub_8bit(AF.getHighValue(), value, &AF, 0, true);
  AF.setHighValue(result);
  return 8;
}

int CPU::sbc_A_r(uint8_t const *r) {
  uint8_t result =
      sub_8bit(AF.getHighValue(), *r, &AF, AF.getCarryFlag(), true);
  AF.setHighValue(result);
  return 4;
}

int CPU::sbc_A_n() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  uint8_t result = sub_8bit(AF.getHighValue(), n, &AF, AF.getCarryFlag(), true);
  AF.setHighValue(result);
  return 8;
}

int CPU::sbc_A_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  uint8_t result =
      sub_8bit(AF.getHighValue(), value, &AF, AF.getCarryFlag(), true);
  AF.setHighValue(result);
  return 8;
}

int CPU::inc_r(uint8_t *r) {
  *r = add_8bit(*r, 1, &AF, 0, false);
  return 4;
}

int CPU::inc_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  value = add_8bit(value, 1, &AF, 0, false);
  mmu.write(HL.getFullValue(), value);
  return 12;
}

int CPU::dec_r(uint8_t *r) {
  *r = sub_8bit(*r, 1, &AF, 0, false);
  return 4;
}

int CPU::dec_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  value = sub_8bit(value, 1, &AF, 0, false);
  mmu.write(HL.getFullValue(), value);
  return 12;
}

uint8_t and_8bit(uint8_t operand1, uint8_t operand2, RegisterAF *AF) {
  uint8_t result = operand1 & operand2;
  if (result == 0) {
    AF->setZeroFlag();
  } else {
    AF->clearZeroFlag();
  }

  AF->clearSubtractionFlag();
  AF->setHalfCarryFlag();
  AF->clearCarryFlag();
  return result;
}

int CPU::and_A_r(uint8_t const *r) {
  uint8_t result = and_8bit(AF.getHighValue(), *r, &AF);
  AF.setHighValue(result);
  return 4;
}

int CPU::and_A_n() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);

  uint8_t result = and_8bit(AF.getHighValue(), n, &AF);
  AF.setHighValue(result);
  return 8;
}

int CPU::and_A_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  uint8_t result = and_8bit(AF.getHighValue(), value, &AF);
  AF.setHighValue(result);
  return 8;
}

uint8_t xor_8bit(uint8_t operand1, uint8_t operand2, RegisterAF *AF) {
  uint8_t result = operand1 ^ operand2;
  if (result == 0) {
    AF->setZeroFlag();
  } else {
    AF->clearZeroFlag();
  }

  AF->clearSubtractionFlag();
  AF->clearHalfCarryFlag();
  AF->clearCarryFlag();
  return result;
}

int CPU::xor_A_r(uint8_t const *r) {
  uint8_t result = xor_8bit(AF.getHighValue(), *r, &AF);
  AF.setHighValue(result);
  return 4;
}

int CPU::xor_A_n() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);

  uint8_t result = xor_8bit(AF.getHighValue(), n, &AF);
  AF.setHighValue(result);
  return 8;
}

int CPU::xor_A_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  uint8_t result = xor_8bit(AF.getHighValue(), value, &AF);
  AF.setHighValue(result);
  return 8;
}

uint8_t or_8bit(uint8_t operand1, uint8_t operand2, RegisterAF *AF) {
  uint8_t result = operand1 | operand2;
  if (result == 0) {
    AF->setZeroFlag();
  } else {
    AF->clearZeroFlag();
  }

  AF->clearSubtractionFlag();
  AF->clearHalfCarryFlag();
  AF->clearCarryFlag();
  return result;
}

int CPU::or_A_r(uint8_t const *r) {
  uint8_t result = or_8bit(AF.getHighValue(), *r, &AF);
  AF.setHighValue(result);
  return 4;
}

int CPU::or_A_n() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);

  uint8_t result = or_8bit(AF.getHighValue(), n, &AF);
  AF.setHighValue(result);
  return 8;
}

int CPU::or_A_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  uint8_t result = or_8bit(AF.getHighValue(), value, &AF);
  AF.setHighValue(result);
  return 8;
}

int CPU::cp_A_r(uint8_t const *r) {
  sub_8bit(AF.getHighValue(), *r, &AF, 0, true);
  return 4;
}

int CPU::cp_A_n() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  sub_8bit(AF.getHighValue(), n, &AF, 0, true);
  return 8;
}

int CPU::cp_A_HL() {
  uint8_t value = mmu.read(HL.getFullValue());
  sub_8bit(AF.getHighValue(), value, &AF, 0, true);
  return 8;
}

int CPU::daa_A() {
  throw std::runtime_error("Error: did not implement DAA");
  return 4;
}

int CPU::cpl_A() {
  AF.setHighValue(AF.getHighValue() ^ 0xFF);
  AF.setSubtractionFlag();
  AF.setHalfCarryFlag();
  return 4;
}

// 16 bit arthimetic instructions
int CPU::add_HL_rr(uint16_t const *rr) {
  uint16_t operand1 = HL.getFullValue();
  uint16_t operand2 = *rr;
  uint16_t result = operand1 + operand2;
  AF.clearSubtractionFlag();

  // See:
  // https://stackoverflow.com/questions/57958631/game-boy-half-carry-flag-and-16-bit-instructions-especially-opcode-0xe8
  if (((operand1 & 0x0FFF) + (operand2 & 0x0FFF)) > 0x0FFF) {
    AF.setHalfCarryFlag();
  } else {
    AF.clearHalfCarryFlag();
  }

  if ((operand1 + operand2) > 0xFFFF) {
    AF.setCarryFlag();
  } else {
    AF.clearCarryFlag();
  }

  HL.setFullValue(result);
  return 8;
}

int CPU::inc_rr(uint16_t *rr) {
  *rr = *rr + 1;
  return 8;
}

int CPU::dec_rr(uint16_t *rr) {
  *rr = *rr - 1;
  return 8;
}

int CPU::add_SP_dd_relative() {
  int8_t dd = static_cast<int8_t>(mmu.read(PC.getPCValue()));
  PC.incrementPC(1);
  uint16_t SP_value = SP.getSPValue();

  uint16_t result = dd + SP_value;

  AF.clearZeroFlag();
  AF.clearSubtractionFlag();

  // See:
  // https://stackoverflow.com/questions/57958631/game-boy-half-carry-flag-and-16-bit-instructions-especially-opcode-0xe8
  if ((SP_value & 0x0F) + (dd & 0x0F) > 0x0F) {
    AF.setHalfCarryFlag();
  } else {
    AF.clearHalfCarryFlag();
  }
  if ((SP_value & 0xFF) + (dd & 0xFF) > 0xFF) {
    AF.setCarryFlag();
  } else {
    AF.clearCarryFlag();
  }

  SP.setSP(result);
  return 16;
}

int CPU::ld_HL_SP_dd_relative() {
  int8_t dd = static_cast<int8_t>(mmu.read(PC.getPCValue()));
  PC.incrementPC(1);
  uint8_t SP_value = SP.getSPValue();
  uint16_t result = dd + SP_value;
  AF.clearZeroFlag();
  AF.clearSubtractionFlag();
  // See:
  // https://stackoverflow.com/questions/57958631/game-boy-half-carry-flag-and-16-bit-instructions-especially-opcode-0xe8
  if ((SP_value & 0x0F) + (dd & 0x0F) > 0x0F) {
    AF.setHalfCarryFlag();
  } else {
    AF.clearHalfCarryFlag();
  }
  if ((SP_value & 0xFF) + (dd & 0xFF) > 0xFF) {
    AF.setCarryFlag();
  } else {
    AF.clearCarryFlag();
  }

  HL.setFullValue(result);
  return 12;
}

// Rotate and shift instructions
int CPU::rlca_A() { return 4; }

// Single bit operation instructions
int CPU::bit_n_r(uint8_t const *r) {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);

  if (((*r) >> n) & 0x1) {
    AF.clearZeroFlag();
  } else {
    AF.setZeroFlag();
  }

  AF.clearSubtractionFlag();
  AF.setHalfCarryFlag();

  return 8;
}

int CPU::bit_n_HL() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);

  uint8_t value = mmu.read(HL.getFullValue());
  if ((value >> n) & 0x1) {
    AF.clearZeroFlag();
  } else {
    AF.setZeroFlag();
  }

  AF.clearSubtractionFlag();
  AF.setHalfCarryFlag();

  return 12;
}

int CPU::set_n_r(uint8_t *r) {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  *r = *r | (1 << n);
  return 8;
}

int CPU::set_n_HL() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);

  uint8_t value = mmu.read(HL.getFullValue());
  value = value | (1 << n);
  mmu.write(HL.getFullValue(), value);
  return 16;
}

int CPU::res_n_r(uint8_t *r) {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);
  *r = *r & ~(1 << n);
  return 8;
}

int CPU::res_n_HL() {
  uint8_t n = mmu.read(PC.getPCValue());
  PC.incrementPC(1);

  uint8_t value = mmu.read(HL.getFullValue());
  value = value & ~(1 << n);
  return 16;
}

// CPU control instructions
int CPU::ccf() {
  AF.xorCarryFlag();
  AF.clearSubtractionFlag();
  AF.clearHalfCarryFlag();
  return 4;
}

int CPU::scf() {
  AF.setCarryFlag();
  AF.clearSubtractionFlag();
  AF.clearHalfCarryFlag();
  return 4;
}

int CPU::nop() { return 4; }

int halt() {
  throw std::runtime_error("Did not implement halt!");
  return -1;
}
int stop() {
  throw std::runtime_error("Did not implement stop!");
  return -1;
}

int CPU::di() {
  IME = false;
  return 4;
}

int CPU::ei() {
  IME = true;
  return 4;
}

// Jump instructions
int CPU::jp_nn() {
  uint16_t nn = get_nn(&mmu, &PC);
  PC.setPC(nn);
  return 16;
}

int CPU::jp_HL() {
  PC.setPC(HL.getFullValue());
  return 4;
}

int CPU::jp_f_nn(uint8_t f, uint8_t condition) {
  uint16_t nn = get_nn(&mmu, &PC);
  if (f == condition) {
    PC.setPC(nn);
    return 16;
  }
  return 12;
}

int CPU::jr_PC_dd() {
  int8_t dd = static_cast<int8_t>(mmu.read(PC.getPCValue()));
  PC.setPC(PC.getPCValue() + 1 + dd);
  return 12;
}

int CPU::jr_f_PC_dd(uint8_t f, uint8_t condition) {
  int8_t dd = static_cast<int8_t>(mmu.read(PC.getPCValue()));
  PC.incrementPC(1);
  if (f == condition) {
    PC.setPC(PC.getPCValue() + dd);
    return 12;
  }
  return 8;
}

int CPU::call_nn() {
  uint16_t nn = get_nn(&mmu, &PC);
  SP.decrementSP(1);
  mmu.write(SP.getSPValue(), PC.getPCValue() >> 8);
  SP.decrementSP(1);
  mmu.write(SP.getSPValue(), PC.getPCValue() & 0x00FF);
  PC.setPC(nn);
  return 24;
}

int CPU::call_f_nn(uint8_t f, uint8_t condition) {
  uint16_t nn = get_nn(&mmu, &PC);
  if (f == condition) {
    SP.decrementSP(1);
    mmu.write(SP.getSPValue(), PC.getPCValue() >> 8);
    SP.decrementSP(1);
    mmu.write(SP.getSPValue(), PC.getPCValue() & 0x00FF);
    PC.setPC(nn);
    return 24;
  }
  return 12;
}

int CPU::ret() {
  uint8_t low = mmu.read(SP.getSPValue());
  SP.incrementSP(1);
  uint8_t high = mmu.read(SP.getSPValue());
  SP.incrementSP(1);
  PC.setPC(high << 8 | low);
  return 16;
}

int CPU::ret_f(uint8_t f, uint8_t condition) {
  if (f == condition) {
    uint8_t low = mmu.read(SP.getSPValue());
    SP.incrementSP(1);
    uint8_t high = mmu.read(SP.getSPValue());
    SP.incrementSP(1);
    PC.setPC(high << 8 | low);
    return 20;
  }
  return 8;
}

int CPU::reti() {
  ret();
  IME = true;
  return 16;
}

int CPU::rst_n(uint8_t reset_value) {
  SP.decrementSP(1);
  mmu.write(SP.getSPValue(), PC.getPCValue() >> 8);
  SP.decrementSP(1);
  mmu.write(SP.getSPValue(), PC.getPCValue() & 0x00FF);
  PC.setPC(reset_value);
  return 16;
}

}  // namespace GB
