#include "GB/CPU.hpp"

namespace GB
{
int CPU::ld_rr_nn(uint16_t *rr)
{
    uint16_t nn = get_nn(&mmu, &PC);
    *rr = nn;
    return 12;
}

int CPU::ld_nn_SP()
{
    uint16_t nn = get_nn(&mmu, &PC);

    uint16_t SP_value = SP.getSPValue();
    uint8_t SP_low = SP_value & 0x00FF;
    uint8_t SP_high = SP_value >> 8;

    mmu.write(nn, SP_low);
    mmu.write(nn + 1, SP_high);
    return 20;
}

int CPU::ld_SP_HL()
{
    SP.setSP(HL.getFullValue());
    return 8;
}

int CPU::push_rr(uint16_t *rr)
{
    uint8_t low = *rr & 0x00FF;
    uint8_t high = *rr >> 8;

    SP.decrementSP(1);
    mmu.write(SP.getSPValue(), high);
    SP.decrementSP(1);
    mmu.write(SP.getSPValue(), low);
    return 16;
}

int CPU::pop_rr(uint16_t *rr, bool is_AF)
{
    uint8_t low = mmu.read(SP.getSPValue());
    SP.incrementSP(1);
    uint8_t high = mmu.read(SP.getSPValue());
    SP.incrementSP(1);
    if (is_AF) {
        *rr = high << 8 | (low & 0xF0);
    } else {
        *rr = high << 8 | low;
    }
    return 12;
}

} // namespace GB
