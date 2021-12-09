#include "GB/CPU.hpp"

namespace GB
{
int CPU::ld_r_r(uint8_t *r1, uint8_t const *r2)
{
    *r1 = *r2;
    return 4;
}

int CPU::ld_r_n(uint8_t *r)
{
    *r = mmu.read(PC.getPCValue());
    PC.incrementPC(1);
    return 8;
}

int CPU::ld_r_HL(uint8_t *r)
{
    *r = mmu.read(HL.getFullValue());
    return 8;
}

int CPU::ld_HL_r(uint8_t const *r)
{
    mmu.write(HL.getFullValue(), *r);
    return 8;
}

int CPU::ld_HL_n()
{
    uint8_t n = mmu.read(PC.getPCValue());
    PC.incrementPC(1);
    mmu.write(HL.getFullValue(), n);
    return 12;
}

int CPU::ld_A_BC()
{
    AF.setHighValue(mmu.read(BC.getFullValue()));
    return 8;
}

int CPU::ld_A_DE()
{
    AF.setHighValue(mmu.read(DE.getFullValue()));
    return 8;
}

int CPU::ld_BC_A()
{
    mmu.write(BC.getFullValue(), AF.getHighValue());
    return 8;
}

int CPU::ld_DE_A()
{
    mmu.write(DE.getFullValue(), AF.getHighValue());
    return 8;
}

int CPU::ld_A_nn()
{
    uint16_t nn = get_nn(&mmu, &PC);
    AF.setHighValue(mmu.read(nn));
    return 16;
}

int CPU::ld_nn_A()
{
    uint16_t nn = get_nn(&mmu, &PC);
    mmu.write(nn, AF.getHighValue());
    return 16;
}

int CPU::ld_A_FFC()
{
    uint8_t low = BC.getLowValue();
    uint8_t high = 0xFF;
    uint16_t addr = high << 8 | low;
    AF.setHighValue(mmu.read(addr));
    return 8;
}

int CPU::ld_FFC_A()
{
    uint8_t low = BC.getLowValue();
    uint8_t high = 0xFF;
    uint16_t addr = high << 8 | low;
    mmu.write(addr, AF.getHighValue());
    return 8;
}

int CPU::ld_A_FFn()
{
    uint8_t low = mmu.read(PC.getPCValue());
    PC.incrementPC(1);
    uint8_t high = 0xFF;
    uint16_t addr = high << 8 | low;
    AF.setHighValue(mmu.read(addr));
    return 12;
}

int CPU::ld_FFn_A()
{
    uint8_t low = mmu.read(PC.getPCValue());
    PC.incrementPC(1);
    uint8_t high = 0xFF;
    uint16_t addr = high << 8 | low;
    mmu.write(addr, AF.getHighValue());
    return 12;
}

int CPU::ldd_A_HL()
{
    AF.setHighValue(mmu.read(HL.getFullValue()));
    HL.setFullValue(HL.getFullValue() - 1);
    return 8;
}

int CPU::ldd_HL_A()
{
    mmu.write(HL.getFullValue(), AF.getHighValue());
    HL.setFullValue(HL.getFullValue() - 1);
    return 8;
}

int CPU::ldi_A_HL()
{
    AF.setHighValue(mmu.read(HL.getFullValue()));
    HL.setFullValue(HL.getFullValue() + 1);
    return 8;
}

int CPU::ldi_HL_A()
{
    mmu.write(HL.getFullValue(), AF.getHighValue());
    HL.setFullValue(HL.getFullValue() + 1);
    return 8;
}

} // namespace GB
