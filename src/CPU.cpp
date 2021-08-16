#include "GB/CPU.hpp"

namespace GB {
CPU::CPU() {
  opcode_table = {
      {[this]() { OP_0x00(); }, 1, 4},  {[this]() { OP_0x01(); }, 3, 12},
      {[this]() { OP_0x02(); }, 1, 8},  {[this]() { OP_0x03(); }, 1, 8},
      {[this]() { OP_0x04(); }, 1, 4},  {[this]() { OP_0x05(); }, 1, 4},
      {[this]() { OP_0x06(); }, 2, 8},  {[this]() { OP_0x07(); }, 1, 4},
      {[this]() { OP_0x08(); }, 3, 20}, {[this]() { OP_0x09(); }, 1, 8},
      {[this]() { OP_0x0A(); }, 1, 8},  {[this]() { OP_0x0B(); }, 1, 8},
      {[this]() { OP_0x0C(); }, 1, 4},  {[this]() { OP_0x0C(); }, 1, 4},
      {[this]() { OP_0x0D(); }, 1, 4},  {[this]() { OP_0x0E(); }, 2, 8},
      {[this]() { OP_0x0F(); }, 1, 4},  {[this]() { OP_0x10(); }, 2, 4},
      {[this]() { OP_0x11(); }, 3, 12}, {[this]() { OP_0x12(); }, 1, 8},
      {[this]() { OP_0x13(); }, 1, 8},  {[this]() { OP_0x14(); }, 1, 4},
      {[this]() { OP_0x15(); }, 1, 4},  {[this]() { OP_0x16(); }, 2, 8},
      {[this]() { OP_0x17(); }, 1, 4},  {[this]() { OP_0x18(); }, 2, 12},
      {[this]() { OP_0x19(); }, 1, 8},  {[this]() { OP_0x1A(); }, 1, 8},
      {[this]() { OP_0x1B(); }, 1, 8},  {[this]() { OP_0x1C(); }, 1, 4},
      {[this]() { OP_0x1D(); }, 1, 4},  {[this]() { OP_0x1E(); }, 2, 8},
      {[this]() { OP_0x1F(); }, 1, 4},  {[this]() { OP_0x20(); }, 2, 12},
  };
}

void CPU::OP_0x00() { return; }
}  // namespace GB
