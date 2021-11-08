#include "GB/CPU.hpp"

int main(int argc, char *argv[]) {
  GB::CPU cpu;
  cpu.loadROM(argv[1]);
  if (argc >= 3) {
    cpu.loadState(argv[2]);
  }
  cpu.runEmulator();
}
