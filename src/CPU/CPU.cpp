#include "GB/CPU.hpp"

#include <iomanip>
#include <iostream>

#include "GB/MMU.hpp"

namespace GB {

// Unused function; was used to debug
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
    int clock_cycles;
    if (is_halted) {
      clock_cycles = 4;
    } else {
      clock_cycles = executeOpcode(opcode);
    }

    updateTimers(clock_cycles);
    checkInterrupts();
    if (mmu.read(0xff02) == 0x81) {
      char c = mmu.read(0xff01);
      printf("%c\n", c);
      mmu.write(0xff02, 0x00);
    }
  }
}

void CPU::checkInterrupts() {
  // Necessary check due to halt bug
  if (mmu.isAnyInterruptEnabled() && mmu.isAnyInterruptRequested()) {
    is_halted = false;
  }

  if (!IME) {
    return;
  }

  if (mmu.isVBlankInterruptEnabled() && mmu.isVBlankInterruptRequested()) {
    handleVBlankInterrupt();
  }

  if (mmu.isLCDStatInterruptEnabled() && mmu.isLCDStatInterruptRequested()) {
    handleLCDStatInterrupt();
  }

  if (mmu.isTimerInterruptEnabled() && mmu.isTimerInterruptRequested()) {
    handleTimerInterrupt();
    printStatus();
    throw std::runtime_error("error");
  }

  if (mmu.isSerialInterruptEnabled() && mmu.isSerialInterruptRequested()) {
    handleSerialInterrupt();
  }

  if (mmu.isJoypadInterruptEnabled() && mmu.isJoypadInterruptRequested()) {
    handleJoypadInterrupt();
  }
}

void CPU::handleVBlankInterrupt() {
  IME = false;
  mmu.resetVBlankInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x40);
}

void CPU::handleLCDStatInterrupt() {
  IME = false;
  mmu.resetLCDStatInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x48);
}

void CPU::handleTimerInterrupt() {
  IME = false;
  mmu.resetTimerInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x50);
}

void CPU::handleSerialInterrupt() {
  IME = false;
  mmu.resetSerialInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x58);
}

void CPU::handleJoypadInterrupt() {
  IME = false;
  mmu.resetJoypadInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x60);
}

void CPU::updateTimers(int clock_cycles) {
  updateDivTimer(clock_cycles);
  if (mmu.isTimerEnabled()) {
    updateOtherTimers(clock_cycles);
  }
}

void CPU::updateDivTimer(int clock_cycles) {
  div_timer_count += clock_cycles;
  while (div_timer_count >= CPU_FREQUENCY / DIV_UPDATE_FREQUENCY) {
    div_timer_count = div_timer_count - CPU_FREQUENCY / DIV_UPDATE_FREQUENCY;
    mmu.incrementDividerRegister(1);
  }
}

void CPU::updateOtherTimers(int clock_cycles) {
  int frequency = getClockFrequency();
  timer_count += clock_cycles;
  while (timer_count >= CPU_FREQUENCY / frequency) {
    timer_count = timer_count - CPU_FREQUENCY / frequency;
    if (mmu.willTimerCounterRegisterOverflow()) {
      mmu.resetTimerCounterRegister();
      mmu.setTimerInterrupt();
    } else {
      mmu.incrementTimerCounterRegister(1);
    }
  }
}

int CPU::getClockFrequency() {
  int frequency = 0;
  uint8_t clock_select = mmu.getInputClockSelect();
  switch (clock_select) {
    case 0x00:
      frequency = 4096;
      break;
    case 0x01:
      frequency = 262144;
      break;
    case 0x02:
      frequency = 65536;
      break;
    case 0x03:
      frequency = 16384;
      break;
    default:
      throw std::runtime_error("Error: unknown frequency returned!");
      break;
  }
  return frequency;
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
