#include <GB/Registers.hpp>
#include <cstdio>

int main(void) {
  GB::Registers registers;
  registers.setA(4);
  printf("%d", registers.getA());
  return 0;
}
