#include "GB/CPU.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

#include "GB/GUI.hpp"
#include "GB/MMU.hpp"

namespace GB {

// Unused function; was used to debug
void CPU::printStatus() {
  std::cout << "\nAF = 0x" << std::hex << AF.getFullValue() << "\n";
  std::cout << "BC = 0x" << std::hex << BC.getFullValue() << "\n";
  std::cout << "DE = 0x" << std::hex << DE.getFullValue() << "\n";
  std::cout << "HL = 0x" << std::hex << HL.getFullValue() << "\n";
  std::cout << "SP = 0x" << std::hex << SP.getSPValue() << "\n";
  std::cout << "PC = 0x" << std::hex << PC.getPCValue() - 1 << "\n";
}

CPU::CPU() : mmu{MMU()}, ppu{PPU(&mmu)} {
  // See: https://gbdev.io/pandocs/Power_Up_Sequence.html
  PC.setPC(0x0100);
  AF.setFullValue(0x01B0);
  BC.setFullValue(0x0013);
  DE.setFullValue(0x00D8);
  HL.setFullValue(0x014D);
  SP.setSP(0xFFFE);
}

void CPU::loadROM(const char *rom_path) { mmu.loadROM(rom_path); }

void CPU::saveState(const char *state_path) {
  std::ofstream save_state_file(state_path, std::ios::binary);
  if (!save_state_file) {
    throw std::runtime_error("Could not write to save state file");
  }
  uint16_t register_value = AF.getFullValue();
  save_state_file.write(reinterpret_cast<char *>(&register_value),
                        sizeof(uint16_t));
  register_value = BC.getFullValue();
  save_state_file.write(reinterpret_cast<char *>(&register_value),
                        sizeof(uint16_t));
  register_value = DE.getFullValue();
  save_state_file.write(reinterpret_cast<char *>(&register_value),
                        sizeof(uint16_t));
  register_value = HL.getFullValue();
  save_state_file.write(reinterpret_cast<char *>(&register_value),
                        sizeof(uint16_t));
  register_value = SP.getSPValue();
  save_state_file.write(reinterpret_cast<char *>(&register_value),
                        sizeof(uint16_t));
  register_value = PC.getPCValue();
  save_state_file.write(reinterpret_cast<char *>(&register_value),
                        sizeof(uint16_t));
  save_state_file.write(reinterpret_cast<char *>(&IME), sizeof(bool));
  save_state_file.write(reinterpret_cast<char *>(&ran_instr), sizeof(bool));
  save_state_file.write(reinterpret_cast<char *>(&is_halted), sizeof(bool));
  save_state_file.write(reinterpret_cast<char *>(&div_timer_count),
                        sizeof(int));
  save_state_file.write(reinterpret_cast<char *>(&timer_count), sizeof(int));
  save_state_file.write(reinterpret_cast<char *>(&render_cycles), sizeof(int));
  save_state_file.write(reinterpret_cast<char *>(&input_cycles), sizeof(int));
  mmu.saveState(save_state_file);
  ppu.saveState(save_state_file);
  save_state_file.close();
}

void CPU::loadState(const char *state_path) {
  std::ifstream save_state_file(state_path, std::ios::binary);
  if (!save_state_file) {
    throw std::runtime_error("Could not read save state file");
  }

  uint16_t register_value;
  save_state_file.read(reinterpret_cast<char *>(&register_value),
                       sizeof(uint16_t));
  AF.setFullValue(register_value);
  save_state_file.read(reinterpret_cast<char *>(&register_value),
                       sizeof(uint16_t));
  BC.setFullValue(register_value);
  save_state_file.read(reinterpret_cast<char *>(&register_value),
                       sizeof(uint16_t));
  DE.setFullValue(register_value);
  save_state_file.read(reinterpret_cast<char *>(&register_value),
                       sizeof(uint16_t));
  HL.setFullValue(register_value);
  save_state_file.read(reinterpret_cast<char *>(&register_value),
                       sizeof(uint16_t));
  SP.setSP(register_value);
  save_state_file.read(reinterpret_cast<char *>(&register_value),
                       sizeof(uint16_t));
  PC.setPC(register_value);
  save_state_file.read(reinterpret_cast<char *>(&IME), sizeof(bool));
  save_state_file.read(reinterpret_cast<char *>(&ran_instr), sizeof(bool));
  save_state_file.read(reinterpret_cast<char *>(&is_halted), sizeof(bool));
  save_state_file.read(reinterpret_cast<char *>(&div_timer_count), sizeof(int));
  save_state_file.read(reinterpret_cast<char *>(&timer_count), sizeof(int));
  save_state_file.read(reinterpret_cast<char *>(&render_cycles), sizeof(int));
  save_state_file.read(reinterpret_cast<char *>(&input_cycles), sizeof(int));
  mmu.loadState(save_state_file);
  ppu.loadState(save_state_file);
  save_state_file.close();
}

void CPU::runEmulator() {
  GUI gui;
  gui.init(ppu.getLCDViewportWidth(), ppu.getLCDViewportHeight());
  while (!gui.shouldQuit()) {
    updateEmulatorState();

    if (input_cycles > kInputPollingFrequency) {
      input_cycles = 0;
      JoyPadButton input = gui.getKeyboardInput();
      mmu.updateJoyPadState(input);
    }

    if (render_cycles > kRenderingFrequency) {
      render_cycles = 0;
      gui.update(ppu.getDisplayBuffer(), ppu.getDisplayBufferPitch());
    }
  }

  if (gui.shouldSave()) {
    const char *save_state_filename{"test.sav"};
    saveState(save_state_filename);
  }
}

void CPU::updateEmulatorState() {
  int clock_cycles = updateCPU();

  render_cycles += clock_cycles;
  input_cycles += clock_cycles;
  ppu.updatePPU(clock_cycles);
  updateTimers(clock_cycles);
  checkInterrupts();
}

int CPU::updateCPU() {
  int clock_cycles = 0;
  if (is_halted) {
    clock_cycles = 4;
  } else {
    uint8_t opcode = mmu.read(PC.getPCValue());
    PC.incrementPC(1);
    clock_cycles = executeOpcode(opcode);
  }
  return clock_cycles;
}

int CPU::checkInterrupts() {
  // Necessary check due to halt bug
  if (mmu.isAnyInterruptEnabled() && mmu.isAnyInterruptRequested()) {
    is_halted = false;
  }

  if (!IME) {
    return 0;
  }

  int cycles = 0;
  if (mmu.isVBlankInterruptEnabled() && mmu.isVBlankInterruptRequested()) {
    cycles += handleVBlankInterrupt();
  }

  if (mmu.isLCDStatInterruptEnabled() && mmu.isLCDStatInterruptRequested()) {
    cycles += handleLCDStatInterrupt();
  }

  if (mmu.isTimerInterruptEnabled() && mmu.isTimerInterruptRequested()) {
    cycles += handleTimerInterrupt();
  }

  if (mmu.isSerialInterruptEnabled() && mmu.isSerialInterruptRequested()) {
    cycles += handleSerialInterrupt();
  }

  if (mmu.isJoypadInterruptEnabled() && mmu.isJoypadInterruptRequested()) {
    cycles += handleJoypadInterrupt();
  }
  return cycles;
}

int CPU::handleVBlankInterrupt() {
  IME = false;
  mmu.resetVBlankInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x40);
  return 20;
}

int CPU::handleLCDStatInterrupt() {
  IME = false;
  mmu.resetLCDStatInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x48);
  return 20;
}

int CPU::handleTimerInterrupt() {
  IME = false;
  mmu.resetTimerInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x50);
  return 20;
}

int CPU::handleSerialInterrupt() {
  IME = false;
  mmu.resetSerialInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x58);
  return 20;
}

int CPU::handleJoypadInterrupt() {
  IME = false;
  mmu.resetJoypadInterruptRequest();
  push_rr(PC.getPC());
  PC.setPC(0x60);
  return 20;
}

void CPU::updateTimers(int clock_cycles) {
  updateDivTimer(clock_cycles);
  if (mmu.isTimerEnabled()) {
    updateOtherTimers(clock_cycles);
  }
}

void CPU::updateDivTimer(int clock_cycles) {
  div_timer_count += clock_cycles;
  while (div_timer_count >= kCPUFrequency / kDivUpdateFrequency) {
    div_timer_count = div_timer_count - kCPUFrequency / kDivUpdateFrequency;
    mmu.incrementDividerRegister(1);
  }
}

void CPU::updateOtherTimers(int clock_cycles) {
  int frequency = getClockFrequency();
  timer_count += clock_cycles;
  while (timer_count >= kCPUFrequency / frequency) {
    timer_count = timer_count - kCPUFrequency / frequency;
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
