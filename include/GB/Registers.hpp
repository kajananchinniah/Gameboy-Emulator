#ifndef INCLUDE_GB_REGISTERS_HPP_
#define INCLUDE_GB_REGISTERS_HPP_

#include <cstdint>

namespace GB {

class Registers {
 public:
  uint8_t getA();
  void setA(uint8_t data);

  uint8_t getB();
  void setB(uint8_t data);

  uint8_t getC();
  void setC(uint8_t data);

  uint8_t getD();
  void setD(uint8_t data);

  uint8_t getE();
  void setE(uint8_t data);

  uint8_t getF();
  void setF(uint8_t data);

  uint8_t getH();
  void setH(uint8_t data);

  uint8_t getL();
  void setL(uint8_t data);

  uint16_t getAF();
  void setAF(uint16_t data);

  uint16_t getBC();
  void setBC(uint16_t data);

  uint16_t getDE();
  void setDE(uint16_t data);

  uint16_t getHL();
  void setHL(uint16_t data);

  uint8_t getZeroFlag();
  void setZeroFlag();
  void clearZeroFlag();

  uint8_t getSubtractionFlag();
  void setSubtractionFlag();
  void clearSubtractionFlag();

  uint8_t getHalfCarryFlag();
  void setHalfCarryFlag();
  void clearHalfCarryFlag();

  uint8_t getCarryFlag();
  void setCarryFlag();
  void clearCarryFlag();

  void incrementPC(uint16_t amount);
  void decrementPC(uint16_t amount);
  void setPC(uint16_t data);
  uint16_t getPC();

  void incrementSP(uint16_t amount);
  void decrementSP(uint16_t amount);
  void setSP(uint16_t data);
  uint16_t getSP();

 private:
  uint8_t A = 0x0;
  uint8_t B = 0x0;
  uint8_t C = 0x0;
  uint8_t D = 0x0;
  uint8_t E = 0x0;
  uint8_t F = 0x0;
  uint8_t H = 0x0;
  uint8_t L = 0x0;
  uint16_t PC = 0x0;
  uint16_t SP = 0x0;

  uint16_t create16BitRegister(uint8_t high, uint8_t low);
  void write16BitData(uint16_t, uint8_t *high, uint8_t *low);
  uint8_t getBit(uint8_t reg, uint8_t position);
  void setBit(uint8_t *reg, uint8_t position);
  void clearBit(uint8_t *reg, uint8_t position);
};
}  // namespace GB

#endif  // INCLUDE_GB_REGISTERS_HPP_
