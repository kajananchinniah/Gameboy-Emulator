#include "GB/CPU.hpp"

namespace
{
uint8_t rl_base(uint8_t value, uint8_t value_bit7, uint8_t or_operand,
                GB::RegisterAF *AF)
{
    uint8_t result = value << 1 | or_operand;
    if (value_bit7 == 1) {
        AF->setCarryFlag();
    } else {
        AF->clearCarryFlag();
    }

    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->clearSubtractionFlag();
    AF->clearHalfCarryFlag();
    return result;
}

uint8_t rr_base(uint8_t value, uint8_t value_bit0, uint8_t or_operand,
                GB::RegisterAF *AF)
{
    uint8_t result = value >> 1 | (or_operand << 7);
    if (value_bit0 == 1) {
        AF->setCarryFlag();
    } else {
        AF->clearCarryFlag();
    }

    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->clearSubtractionFlag();
    AF->clearHalfCarryFlag();
    return result;
}

uint8_t sla_base(uint8_t value, uint8_t value_bit7, GB::RegisterAF *AF)
{
    uint8_t result = value << 1;
    if (value_bit7 == 1) {
        AF->setCarryFlag();
    } else {
        AF->clearCarryFlag();
    }

    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->clearSubtractionFlag();
    AF->clearHalfCarryFlag();
    return result;
}

uint8_t swap_base(uint8_t value, GB::RegisterAF *AF)
{
    uint8_t low = (value >> 4) & 0x0F;
    uint8_t high = (value << 4) & 0xF0;
    uint8_t result = high | low;
    AF->clearCarryFlag();
    AF->clearSubtractionFlag();
    AF->clearHalfCarryFlag();
    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }
    return high | low;
}

uint8_t sra_base(uint8_t value, uint8_t value_bit0, uint8_t value_bit7,
                 GB::RegisterAF *AF)
{
    uint8_t result = (value >> 1) | (value_bit7 << 7);

    if (value_bit0 == 1) {
        AF->setCarryFlag();
    } else {
        AF->clearCarryFlag();
    }

    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->clearSubtractionFlag();
    AF->clearHalfCarryFlag();
    return result;
}

uint8_t srl_base(uint8_t value, uint8_t value_bit0, GB::RegisterAF *AF)
{
    uint8_t result = value >> 1;
    if (value_bit0 == 1) {
        AF->setCarryFlag();
    } else {
        AF->clearCarryFlag();
    }

    if (result == 0) {
        AF->setZeroFlag();
    } else {
        AF->clearZeroFlag();
    }

    AF->clearSubtractionFlag();
    AF->clearHalfCarryFlag();
    return result;
}

} // namespace

namespace GB
{
int CPU::rlca_A()
{
    uint8_t A = AF.getHighValue();
    uint8_t A_bit7 = (A >> 7) & 0x01;
    AF.setHighValue(A << 1 | A_bit7);

    if (A_bit7 == 1) {
        AF.setCarryFlag();
    } else {
        AF.clearCarryFlag();
    }

    AF.clearZeroFlag();
    AF.clearSubtractionFlag();
    AF.clearHalfCarryFlag();
    return 4;
}

int CPU::rla_A()
{
    uint8_t A = AF.getHighValue();
    uint8_t A_bit7 = (A >> 7) & 0x01;
    uint8_t carry = AF.getCarryFlag();

    AF.setHighValue(A << 1 | carry);

    if (A_bit7 == 1) {
        AF.setCarryFlag();
    } else {
        AF.clearCarryFlag();
    }

    AF.clearZeroFlag();
    AF.clearSubtractionFlag();
    AF.clearHalfCarryFlag();
    return 4;
}

int CPU::rrca_A()
{
    uint8_t A = AF.getHighValue();
    uint8_t A_bit0 = A & 0x01;
    AF.setHighValue((A >> 1) | (A_bit0 << 7));

    if (A_bit0 == 1) {
        AF.setCarryFlag();
    } else {
        AF.clearCarryFlag();
    }

    AF.clearZeroFlag();
    AF.clearSubtractionFlag();
    AF.clearHalfCarryFlag();
    return 4;
}

int CPU::rra_A()
{
    uint8_t A = AF.getHighValue();
    uint8_t A_bit0 = A & 0x01;
    uint8_t carry = AF.getCarryFlag();

    AF.setHighValue((A >> 1) | (carry << 7));

    if (A_bit0 == 1) {
        AF.setCarryFlag();
    } else {
        AF.clearCarryFlag();
    }

    AF.clearZeroFlag();
    AF.clearSubtractionFlag();
    AF.clearHalfCarryFlag();
    return 4;
}

int CPU::rlc_r(uint8_t *r)
{
    uint8_t value = *r;
    uint8_t value_bit7 = (value >> 7) & 0x01;
    *r = rl_base(value, value_bit7, value_bit7, &AF);
    return 8;
}

int CPU::rlc_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t value_bit7 = (value >> 7) & 0x01;
    uint8_t result = rl_base(value, value_bit7, value_bit7, &AF);
    mmu.write(HL.getFullValue(), result);
    return 16;
}

int CPU::rl_r(uint8_t *r)
{
    uint8_t value = *r;
    uint8_t value_bit7 = (value >> 7) & 0x01;
    uint8_t carry = AF.getCarryFlag();
    *r = rl_base(value, value_bit7, carry, &AF);
    return 8;
}

int CPU::rl_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t value_bit7 = (value >> 7) & 0x01;
    uint8_t carry = AF.getCarryFlag();
    uint8_t result = rl_base(value, value_bit7, carry, &AF);
    mmu.write(HL.getFullValue(), result);
    return 16;
}

int CPU::rrc_r(uint8_t *r)
{
    uint8_t value = *r;
    uint8_t value_bit0 = value & 0x01;
    *r = rr_base(value, value_bit0, value_bit0, &AF);
    return 8;
}

int CPU::rrc_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t value_bit0 = value & 0x01;
    uint8_t result = rr_base(value, value_bit0, value_bit0, &AF);
    mmu.write(HL.getFullValue(), result);
    return 16;
}

int CPU::rr_r(uint8_t *r)
{
    uint8_t value = *r;
    uint8_t value_bit0 = value & 0x01;
    uint8_t carry = AF.getCarryFlag();
    *r = rr_base(value, value_bit0, carry, &AF);
    return 8;
}

int CPU::rr_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t value_bit0 = value & 0x01;
    uint8_t carry = AF.getCarryFlag();
    uint8_t result = rr_base(value, value_bit0, carry, &AF);
    mmu.write(HL.getFullValue(), result);
    return 16;
}

int CPU::sla_r(uint8_t *r)
{
    uint8_t value = *r;
    uint8_t value_bit7 = (value >> 7) & 0x01;
    *r = sla_base(value, value_bit7, &AF);
    return 8;
}

int CPU::sla_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t value_bit7 = (value >> 7) & 0x01;
    uint8_t result = sla_base(value, value_bit7, &AF);
    mmu.write(HL.getFullValue(), result);
    return 16;
}

int CPU::swap_r(uint8_t *r)
{
    *r = swap_base(*r, &AF);
    return 8;
}

int CPU::swap_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t result = swap_base(value, &AF);
    mmu.write(HL.getFullValue(), result);
    return 16;
}

int CPU::sra_r(uint8_t *r)
{
    uint8_t value = *r;
    uint8_t value_bit0 = value & 0x01;
    uint8_t value_bit7 = (value >> 7) & 0x01;
    *r = sra_base(value, value_bit0, value_bit7, &AF);
    return 8;
}

int CPU::sra_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t value_bit0 = value & 0x01;
    uint8_t value_bit7 = (value >> 7) & 0x01;
    uint8_t result = sra_base(value, value_bit0, value_bit7, &AF);
    mmu.write(HL.getFullValue(), result);
    return 16;
}

int CPU::srl_r(uint8_t *r)
{
    uint8_t value = *r;
    uint8_t value_bit0 = value & 0x01;
    *r = srl_base(value, value_bit0, &AF);
    return 8;
}

int CPU::srl_HL()
{
    uint8_t value = mmu.read(HL.getFullValue());
    uint8_t value_bit0 = value & 0x01;
    uint8_t result = srl_base(value, value_bit0, &AF);
    mmu.write(HL.getFullValue(), result);
    return 16;
}
} // namespace GB
