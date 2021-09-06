#include "GB/CPU.hpp"

#include <iomanip>
#include <iostream>

#include "GB/MMU.hpp"

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
  // See: https://gbdev.gg8.se/wiki/articles/Power_Up_Sequence
  PC.setPC(0x0100);
  AF.setFullValue(0x01B0);
  BC.setFullValue(0x0013);
  DE.setFullValue(0x00D8);
  HL.setFullValue(0x014D);
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
      printf("%c\n", c);
      mmu.write(0xff02, 0x00);
    }
  }
}

int CPU::unsupportedOpcode(uint8_t opcode, std::string prefix) {
  std::stringstream error_stream;
  error_stream << "Unknown opcode: 0x" << prefix << std::setfill('0')
               << std::setw(2) << std::hex << static_cast<int>(opcode)
               << " at 0x" << std::hex << static_cast<int>(PC.getPCValue());
  throw std::runtime_error(error_stream.str());
  return -1;
}

}  // namespace GB
