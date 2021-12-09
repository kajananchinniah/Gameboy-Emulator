#include "GB/CPU.hpp"

namespace GB
{
int CPU::jp_nn()
{
    uint16_t nn = get_nn(&mmu, &PC);
    PC.setPC(nn);
    return 16;
}

int CPU::jp_HL()
{
    PC.setPC(HL.getFullValue());
    return 4;
}

int CPU::jp_f_nn(uint8_t f, uint8_t condition)
{
    uint16_t nn = get_nn(&mmu, &PC);
    if (f == condition) {
        PC.setPC(nn);
        return 16;
    }
    return 12;
}

int CPU::jr_PC_dd()
{
    int8_t dd = static_cast<int8_t>(mmu.read(PC.getPCValue()));
    PC.setPC(PC.getPCValue() + 1 + dd);
    return 12;
}

int CPU::jr_f_PC_dd(uint8_t f, uint8_t condition)
{
    int8_t dd = static_cast<int8_t>(mmu.read(PC.getPCValue()));
    PC.incrementPC(1);
    if (f == condition) {
        PC.setPC(PC.getPCValue() + dd);
        return 12;
    }
    return 8;
}

int CPU::call_nn()
{
    uint16_t nn = get_nn(&mmu, &PC);
    SP.decrementSP(1);
    mmu.write(SP.getSPValue(), PC.getPCValue() >> 8);
    SP.decrementSP(1);
    mmu.write(SP.getSPValue(), PC.getPCValue() & 0x00FF);
    PC.setPC(nn);
    return 24;
}

int CPU::call_f_nn(uint8_t f, uint8_t condition)
{
    uint16_t nn = get_nn(&mmu, &PC);
    if (f == condition) {
        SP.decrementSP(1);
        mmu.write(SP.getSPValue(), PC.getPCValue() >> 8);
        SP.decrementSP(1);
        mmu.write(SP.getSPValue(), PC.getPCValue() & 0x00FF);
        PC.setPC(nn);
        return 24;
    }
    return 12;
}

int CPU::ret()
{
    uint8_t low = mmu.read(SP.getSPValue());
    SP.incrementSP(1);
    uint8_t high = mmu.read(SP.getSPValue());
    SP.incrementSP(1);
    PC.setPC(high << 8 | low);
    return 16;
}

int CPU::ret_f(uint8_t f, uint8_t condition)
{
    if (f == condition) {
        uint8_t low = mmu.read(SP.getSPValue());
        SP.incrementSP(1);
        uint8_t high = mmu.read(SP.getSPValue());
        SP.incrementSP(1);
        PC.setPC(high << 8 | low);
        return 20;
    }
    return 8;
}

int CPU::reti()
{
    ret();
    IME = true;
    return 16;
}

int CPU::rst_n(uint8_t reset_value)
{
    SP.decrementSP(1);
    mmu.write(SP.getSPValue(), PC.getPCValue() >> 8);
    SP.decrementSP(1);
    mmu.write(SP.getSPValue(), PC.getPCValue() & 0x00FF);
    PC.setPC(reset_value);
    return 16;
}
} // namespace GB
