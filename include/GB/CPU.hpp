#ifndef INCLUDE_GB_CPU_HPP_
#define INCLUDE_GB_CPU_HPP_

#include "GB/MMU.hpp"
#include "GB/Registers.hpp"

namespace GB {
class CPU {
 public:

 private:
  Registers registers;
  MMU mmu;
};

}  // namespace GB

#endif  // INCLUDE_GB_CPU_HPP_
