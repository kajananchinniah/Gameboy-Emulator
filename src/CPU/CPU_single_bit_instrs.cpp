#include "GB/CPU.hpp"

namespace
{
void bit_base(uint8_t n, uint8_t test_value, GB::RegisterAF *AF)
{
    if ((test_value >> n) & 0x01) {
        AF->clearZeroFlag();
    } else {
        AF->setZeroFlag();
    }

    AF->clearSubtractionFlag();
    AF->setHalfCarryFlag();
}

uint8_t set_base(uint8_t n, uint8_t value)
{
    return value | (1 << n);
}

uint8_t res_base(uint8_t n, uint8_t value)
{
    return value & ~(1 << n);
}
} // namespace

namespace GB
{
int CPU::bit_n_r(uint8_t n, uint8_t const *r)
{
    bit_base(n, *r, &AF);
    return 8;
}

int CPU::bit_n_HL(uint8_t n)
{
    uint8_t value = mmu.read(HL.getFullValue());
    bit_base(n, value, &AF);
    return 12;
}

int CPU::set_n_r(uint8_t n, uint8_t *r)
{
    *r = set_base(n, *r);
    return 8;
}

int CPU::set_n_HL(uint8_t n)
{
    uint8_t value = mmu.read(HL.getFullValue());
    value = set_base(n, value);
    mmu.write(HL.getFullValue(), value);
    return 16;
}

int CPU::res_n_r(uint8_t n, uint8_t *r)
{
    *r = res_base(n, *r);
    return 8;
}

int CPU::res_n_HL(uint8_t n)
{
    uint8_t value = mmu.read(HL.getFullValue());
    value = res_base(n, value);
    mmu.write(HL.getFullValue(), value);
    return 16;
}
} // namespace GB
