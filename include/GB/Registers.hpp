#ifndef INCLUDE_GB_REGISTERS_HPP_
#define INCLUDE_GB_REGISTERS_HPP_

#include <cstdint>

namespace GB {

class Register {
 public:
  uint16_t *getFullRegister();
  uint8_t *getHighRegister();
  uint8_t *getLowRegister();
  uint8_t getHighValue();
  uint8_t getLowValue();
  uint16_t getFullValue();
  void setHighValue(uint8_t data);
  void setLowValue(uint8_t data);
  void setFullValue(uint16_t data);

 protected:
  union Data {
    struct {
      uint8_t low = 0x00;
      uint8_t high = 0x00;
    };
    uint16_t full;
  };

  Data data_;
};

class RegisterAF : public Register {
 public:
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
  void xorCarryFlag();

 private:
  uint8_t getBit(uint8_t reg, uint8_t position);
  void setBit(uint8_t *reg, uint8_t position);
  void clearBit(uint8_t *reg, uint8_t position);
};

class ProgramCounter {
 public:
  void incrementPC(uint16_t amount);
  void decrementPC(uint16_t amount);
  void setPC(uint16_t data);
  uint16_t getPCValue();
  uint16_t *getPC();

 private:
  uint16_t PC;
};

class StackPointer {
 public:
  void incrementSP(uint16_t amount);
  void decrementSP(uint16_t amount);
  void setSP(uint16_t data);
  uint16_t getSPValue();
  uint16_t *getSP();

 private:
  uint16_t SP;
};
}  // namespace GB

#endif  // INCLUDE_GB_REGISTERS_HPP_
