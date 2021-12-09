#include "GB/CPU.hpp"
namespace
{
uint8_t add_8bit(uint8_t operand1, uint8_t operand2, GB::RegisterAF *AF,
                 uint8_t carry, bool modify_carry)
{
    uint8_t result = operand1 + operand2 + carry;
    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->clearSubtractionFlag();

    if (((operand1 & 0x0F) + (operand2 & 0x0F) + carry) > 0x0F) {
        AF->setHalfCarryFlag();
    } else {
        AF->clearHalfCarryFlag();
    }

    if (!modify_carry) {
        return result;
    }

    if ((operand1 + operand2 + carry) > 0xFF) {
        AF->setCarryFlag();
    } else {
        AF->clearCarryFlag();
    }
    return result;
}

uint8_t sub_8bit(uint8_t operand1, uint8_t operand2, GB::RegisterAF *AF,
                 uint8_t carry, bool modify_carry)
{
    uint8_t result = operand1 - operand2 - carry;
    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->setSubtractionFlag();

    if ((operand1 & 0x0F) < (operand2 & 0x0F) + carry) {
        AF->setHalfCarryFlag();
    } else {
        AF->clearHalfCarryFlag();
    }

    if (!modify_carry) {
        return result;
    }

    if (operand1 < operand2 + carry) {
        AF->setCarryFlag();
    } else {
        AF->clearCarryFlag();
    }
    return result;
}

uint8_t and_8bit(uint8_t operand1, uint8_t operand2, GB::RegisterAF *AF)
{
    uint8_t result = operand1 & operand2;
    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->clearSubtractionFlag();
    AF->setHalfCarryFlag();
    AF->clearCarryFlag();
    return result;
}

uint8_t xor_8bit(uint8_t operand1, uint8_t operand2, GB::RegisterAF *AF)
{
    uint8_t result = operand1 ^ operand2;
    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->clearSubtractionFlag();
    AF->clearHalfCarryFlag();
    AF->clearCarryFlag();
    return result;
}

uint8_t or_8bit(uint8_t operand1, uint8_t operand2, GB::RegisterAF *AF)
{
    uint8_t result = operand1 | operand2;
    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->clearSubtractionFlag();
    AF->clearHalfCarryFlag();
    AF->clearCarryFlag();
    return result;
}

} // namespace

namespace GB
{
int CPU::add_A_r(uint8_t const *r)
{
    uint8_t result = add_8bit(AF.getHighValue(), *r, &AF, 0, true);
    AF.setHighValue(result);
    return 4;
}

int CPU::add_A_n()
{
    uint8_t n = mmu.read(PC.getPCValue());
    PC.incrementPC(1);

    uint8_t result = add_8bit(AF.getHighValue(), n, &AF, 0, true);
    AF.setHighValue(result);
    return 8;
}

int CPU::add_A_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t result = add_8bit(AF.getHighValue(), value, &AF, 0, true);
    AF.setHighValue(result);
    return 8;
}
int CPU::adc_A_r(uint8_t const *r)
{
    uint8_t result =
        add_8bit(AF.getHighValue(), *r, &AF, AF.getCarryFlag(), true);
    AF.setHighValue(result);
    return 4;
}

int CPU::adc_A_n()
{
    uint8_t n = mmu.read(PC.getPCValue());
    PC.incrementPC(1);

    uint8_t result =
        add_8bit(AF.getHighValue(), n, &AF, AF.getCarryFlag(), true);
    AF.setHighValue(result);
    return 8;
}

int CPU::adc_A_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t result =
        add_8bit(AF.getHighValue(), value, &AF, AF.getCarryFlag(), true);
    AF.setHighValue(result);
    return 8;
}

int CPU::sub_A_r(uint8_t const *r)
{
    uint8_t result = sub_8bit(AF.getHighValue(), *r, &AF, 0, true);
    AF.setHighValue(result);
    return 4;
}

int CPU::sub_A_n()
{
    uint8_t n = mmu.read(PC.getPCValue());
    PC.incrementPC(1);
    uint8_t result = sub_8bit(AF.getHighValue(), n, &AF, 0, true);
    AF.setHighValue(result);
    return 8;
}

int CPU::sub_A_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t result = sub_8bit(AF.getHighValue(), value, &AF, 0, true);
    AF.setHighValue(result);
    return 8;
}

int CPU::sbc_A_r(uint8_t const *r)
{
    uint8_t result =
        sub_8bit(AF.getHighValue(), *r, &AF, AF.getCarryFlag(), true);
    AF.setHighValue(result);
    return 4;
}

int CPU::sbc_A_n()
{
    uint8_t n = mmu.read(PC.getPCValue());
    PC.incrementPC(1);
    uint8_t result =
        sub_8bit(AF.getHighValue(), n, &AF, AF.getCarryFlag(), true);
    AF.setHighValue(result);
    return 8;
}

int CPU::sbc_A_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t result =
        sub_8bit(AF.getHighValue(), value, &AF, AF.getCarryFlag(), true);
    AF.setHighValue(result);
    return 8;
}

int CPU::inc_r(uint8_t *r)
{
    *r = add_8bit(*r, 1, &AF, 0, false);
    return 4;
}

int CPU::inc_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    value = add_8bit(value, 1, &AF, 0, false);
    mmu.write(HL.getFullValue(), value);
    return 12;
}

int CPU::dec_r(uint8_t *r)
{
    *r = sub_8bit(*r, 1, &AF, 0, false);
    return 4;
}

int CPU::dec_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    value = sub_8bit(value, 1, &AF, 0, false);
    mmu.write(HL.getFullValue(), value);
    return 12;
}

int CPU::and_A_r(uint8_t const *r)
{
    uint8_t result = and_8bit(AF.getHighValue(), *r, &AF);
    AF.setHighValue(result);
    return 4;
}

int CPU::and_A_n()
{
    uint8_t n = mmu.read(PC.getPCValue());
    PC.incrementPC(1);

    uint8_t result = and_8bit(AF.getHighValue(), n, &AF);
    AF.setHighValue(result);
    return 8;
}

int CPU::and_A_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t result = and_8bit(AF.getHighValue(), value, &AF);
    AF.setHighValue(result);
    return 8;
}

int CPU::xor_A_r(uint8_t const *r)
{
    uint8_t result = xor_8bit(AF.getHighValue(), *r, &AF);
    AF.setHighValue(result);
    return 4;
}

int CPU::xor_A_n()
{
    uint8_t n = mmu.read(PC.getPCValue());
    PC.incrementPC(1);

    uint8_t result = xor_8bit(AF.getHighValue(), n, &AF);
    AF.setHighValue(result);
    return 8;
}

int CPU::xor_A_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t result = xor_8bit(AF.getHighValue(), value, &AF);
    AF.setHighValue(result);
    return 8;
}

int CPU::or_A_r(uint8_t const *r)
{
    uint8_t result = or_8bit(AF.getHighValue(), *r, &AF);
    AF.setHighValue(result);
    return 4;
}

int CPU::or_A_n()
{
    uint8_t n = mmu.read(PC.getPCValue());
    PC.incrementPC(1);

    uint8_t result = or_8bit(AF.getHighValue(), n, &AF);
    AF.setHighValue(result);
    return 8;
}

int CPU::or_A_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t result = or_8bit(AF.getHighValue(), value, &AF);
    AF.setHighValue(result);
    return 8;
}

int CPU::cp_A_r(uint8_t const *r)
{
    sub_8bit(AF.getHighValue(), *r, &AF, 0, true);
    return 4;
}

int CPU::cp_A_n()
{
    uint8_t n = mmu.read(PC.getPCValue());
    PC.incrementPC(1);
    sub_8bit(AF.getHighValue(), n, &AF, 0, true);
    return 8;
}

int CPU::cp_A_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    sub_8bit(AF.getHighValue(), value, &AF, 0, true);
    return 8;
}

int CPU::daa_A()
{
    // See: http://archive.nes.science/nesdev-forums/f20/t15944.xhtml
    if (AF.getSubtractionFlag()) {
        if (AF.getCarryFlag()) {
            AF.setHighValue(AF.getHighValue() - 0x60);
        }
        if (AF.getHalfCarryFlag()) {
            AF.setHighValue(AF.getHighValue() - 0x06);
        }
    } else {
        if (AF.getCarryFlag() || AF.getHighValue() > 0x99) {
            AF.setHighValue(AF.getHighValue() + 0x60);
            AF.setCarryFlag();
        }

        if (AF.getHalfCarryFlag() || (AF.getHighValue() & 0x0F) > 0x09) {
            AF.setHighValue(AF.getHighValue() + 0x06);
        }
    }

    if (AF.getHighValue() == 0) {
        AF.setZeroFlag();
    } else {
        AF.clearZeroFlag();
    }

    AF.clearHalfCarryFlag();
    return 4;
}

int CPU::cpl_A()
{
    AF.setHighValue(AF.getHighValue() ^ 0xFF);
    AF.setSubtractionFlag();
    AF.setHalfCarryFlag();
    return 4;
}

} // namespace GB
