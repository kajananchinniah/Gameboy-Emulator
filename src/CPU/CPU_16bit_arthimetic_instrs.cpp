#include "GB/CPU.hpp"

namespace GB
{
int CPU::add_HL_rr(uint16_t const *rr)
{
    uint16_t operand1 = HL.getFullValue();
    uint16_t operand2 = *rr;
    uint16_t result = operand1 + operand2;
    AF.clearSubtractionFlag();

    // See:
    // https://stackoverflow.com/questions/57958631/game-boy-half-carry-flag-and-16-bit-instructions-especially-opcode-0xe8
    if (((operand1 & 0x0FFF) + (operand2 & 0x0FFF)) > 0x0FFF) {
        AF.setHalfCarryFlag();
    } else {
        AF.clearHalfCarryFlag();
    }

    if ((operand1 + operand2) > 0xFFFF) {
        AF.setCarryFlag();
    } else {
        AF.clearCarryFlag();
    }

    HL.setFullValue(result);
    return 8;
}

int CPU::inc_rr(uint16_t *rr)
{
    *rr = *rr + 1;
    return 8;
}

int CPU::dec_rr(uint16_t *rr)
{
    *rr = *rr - 1;
    return 8;
}

int CPU::add_SP_dd_relative()
{
    int8_t dd = static_cast<int8_t>(mmu.read(PC.getPCValue()));
    PC.incrementPC(1);
    uint16_t SP_value = SP.getSPValue();

    uint16_t result = dd + SP_value;

    AF.clearZeroFlag();
    AF.clearSubtractionFlag();

    // See:
    // https://stackoverflow.com/questions/57958631/game-boy-half-carry-flag-and-16-bit-instructions-especially-opcode-0xe8
    if ((SP_value & 0x0F) + (dd & 0x0F) > 0x0F) {
        AF.setHalfCarryFlag();
    } else {
        AF.clearHalfCarryFlag();
    }
    if ((SP_value & 0xFF) + (dd & 0xFF) > 0xFF) {
        AF.setCarryFlag();
    } else {
        AF.clearCarryFlag();
    }

    SP.setSP(result);
    return 16;
}

int CPU::ld_HL_SP_dd_relative()
{
    int8_t dd = static_cast<int8_t>(mmu.read(PC.getPCValue()));
    PC.incrementPC(1);
    uint16_t SP_value = SP.getSPValue();
    uint16_t result = dd + SP_value;
    AF.clearZeroFlag();
    AF.clearSubtractionFlag();
    // See:
    // https://stackoverflow.com/questions/57958631/game-boy-half-carry-flag-and-16-bit-instructions-especially-opcode-0xe8
    if ((SP_value & 0x0F) + (dd & 0x0F) > 0x0F) {
        AF.setHalfCarryFlag();
    } else {
        AF.clearHalfCarryFlag();
    }
    if ((SP_value & 0xFF) + (dd & 0xFF) > 0xFF) {
        AF.setCarryFlag();
    } else {
        AF.clearCarryFlag();
    }

    HL.setFullValue(result);
    return 12;
}
} // namespace GB
