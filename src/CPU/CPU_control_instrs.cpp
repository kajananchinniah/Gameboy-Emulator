#include <iostream>

#include "GB/CPU.hpp"

namespace GB {
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

int CPU::halt() {
  is_halted = true;
  return 4;
}

int CPU::stop() {
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
}  // namespace GB
