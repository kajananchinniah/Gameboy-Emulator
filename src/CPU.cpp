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

void CPU::update() {
  while (true) {
    cycle();
  }
}
unsigned int CPU::cycle() {
  if (mmu.read(0xff02) == 0x81) {
    char c = mmu.read(0xff01);
    std::cout << c;
    mmu.write(0xff02, 0x0);
  }

  uint8_t opcode = mmu.read(PC.getPCValue());
  switch (opcode) {
    case 0x00:
      // std::cout << "Executed ! " << static_cast<int>(opcode) << "\n";
      // std::cout << std::hex << PC.getPCValue() << "\n";
      PC.incrementPC(1);
      return 1;
      break;
    case 0x48:
      PC.incrementPC(1);
      return 0;
      break;
    default:
      PC.incrementPC(1);
      std::cout << std::hex << PC.getPCValue() << "\n";
      std::cout << "Unsupported: " << std::hex << (int)opcode
                << "\n";
      exit(-1);
  }
}
}  // namespace GB
