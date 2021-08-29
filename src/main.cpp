#include "GB/CPU.hpp"

int main(int argc, char *argv[]) {
  GB::CPU cpu;
  cpu.loadROM(argv[1]);
  cpu.update();
}
