#include "GB/CPU.hpp"

namespace GB
{
int CPU::executeOpcode(uint8_t opcode)
{
    switch (opcode & 0xF0) {
    case 0x00:
        return execute0x0XTable(opcode);
    case 0x10:
        return execute0x1XTable(opcode);
    case 0x20:
        return execute0x2XTable(opcode);
    case 0x30:
        return execute0x3XTable(opcode);
    case 0x40:
        return execute0x4XTable(opcode);
    case 0x50:
        return execute0x5XTable(opcode);
    case 0x60:
        return execute0x6XTable(opcode);
    case 0x70:
        return execute0x7XTable(opcode);
    case 0x80:
        return execute0x8XTable(opcode);
    case 0x90:
        return execute0x9XTable(opcode);
    case 0xA0:
        return execute0xAXTable(opcode);
    case 0xB0:
        return execute0xBXTable(opcode);
    case 0xC0:
        return execute0xCXTable(opcode);
    case 0xD0:
        return execute0xDXTable(opcode);
    case 0xE0:
        return execute0xEXTable(opcode);
    case 0xF0:
        return execute0xFXTable(opcode);
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0x0XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return nop();
    case 0x01:
        return ld_rr_nn(BC.getFullRegister());
    case 0x02:
        return ld_BC_A();
    case 0x03:
        return inc_rr(BC.getFullRegister());
    case 0x04:
        return inc_r(BC.getHighRegister());
    case 0x05:
        return dec_r(BC.getHighRegister());
    case 0x06:
        return ld_r_n(BC.getHighRegister());
    case 0x07:
        return rlca_A();
    case 0x08:
        return ld_nn_SP();
    case 0x09:
        return add_HL_rr(BC.getFullRegister());
    case 0x0A:
        return ld_A_BC();
    case 0x0B:
        return dec_rr(BC.getFullRegister());
    case 0x0C:
        return inc_r(BC.getLowRegister());
    case 0x0D:
        return dec_r(BC.getLowRegister());
    case 0x0E:
        return ld_r_n(BC.getLowRegister());
    case 0x0F:
        return rrca_A();
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0x1XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return stop();
    case 0x01:
        return ld_rr_nn(DE.getFullRegister());
    case 0x02:
        return ld_DE_A();
    case 0x03:
        return inc_rr(DE.getFullRegister());
    case 0x04:
        return inc_r(DE.getHighRegister());
    case 0x05:
        return dec_r(DE.getHighRegister());
    case 0x06:
        return ld_r_n(DE.getHighRegister());
    case 0x07:
        return rla_A();
    case 0x08:
        return jr_PC_dd();
    case 0x09:
        return add_HL_rr(DE.getFullRegister());
    case 0x0A:
        return ld_A_DE();
    case 0x0B:
        return dec_rr(DE.getFullRegister());
    case 0x0C:
        return inc_r(DE.getLowRegister());
    case 0x0D:
        return dec_r(DE.getLowRegister());
    case 0x0E:
        return ld_r_n(DE.getLowRegister());
    case 0x0F:
        return rra_A();
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0x2XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return jr_f_PC_dd(AF.getZeroFlag(), 0);
    case 0x01:
        return ld_rr_nn(HL.getFullRegister());
    case 0x02:
        return ldi_HL_A();
    case 0x03:
        return inc_rr(HL.getFullRegister());
    case 0x04:
        return inc_r(HL.getHighRegister());
    case 0x05:
        return dec_r(HL.getHighRegister());
    case 0x06:
        return ld_r_n(HL.getHighRegister());
    case 0x07:
        return daa_A();
    case 0x08:
        return jr_f_PC_dd(AF.getZeroFlag(), 1);
    case 0x09:
        return add_HL_rr(HL.getFullRegister());
    case 0x0A:
        return ldi_A_HL();
    case 0x0B:
        return dec_rr(HL.getFullRegister());
    case 0x0C:
        return inc_r(HL.getLowRegister());
    case 0x0D:
        return dec_r(HL.getLowRegister());
    case 0x0E:
        return ld_r_n(HL.getLowRegister());
    case 0x0F:
        return cpl_A();
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0x3XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return jr_f_PC_dd(AF.getCarryFlag(), 0);
    case 0x01:
        return ld_rr_nn(SP.getSP());
    case 0x02:
        return ldd_HL_A();
    case 0x03:
        return inc_rr(SP.getSP());
    case 0x04:
        return inc_HL();
    case 0x05:
        return dec_HL();
    case 0x06:
        return ld_HL_n();
    case 0x07:
        return scf();
    case 0x08:
        return jr_f_PC_dd(AF.getCarryFlag(), 1);
    case 0x09:
        return add_HL_rr(SP.getSP());
    case 0x0A:
        return ldd_A_HL();
    case 0x0B:
        return dec_rr(SP.getSP());
    case 0x0C:
        return inc_r(AF.getHighRegister());
    case 0x0D:
        return dec_r(AF.getHighRegister());
    case 0x0E:
        return ld_r_n(AF.getHighRegister());
    case 0x0F:
        return ccf();
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0x4XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return ld_r_r(BC.getHighRegister(), BC.getHighRegister());
    case 0x01:
        return ld_r_r(BC.getHighRegister(), BC.getLowRegister());
    case 0x02:
        return ld_r_r(BC.getHighRegister(), DE.getHighRegister());
    case 0x03:
        return ld_r_r(BC.getHighRegister(), DE.getLowRegister());
    case 0x04:
        return ld_r_r(BC.getHighRegister(), HL.getHighRegister());
    case 0x05:
        return ld_r_r(BC.getHighRegister(), HL.getLowRegister());
    case 0x06:
        return ld_r_HL(BC.getHighRegister());
    case 0x07:
        return ld_r_r(BC.getHighRegister(), AF.getHighRegister());
    case 0x08:
        return ld_r_r(BC.getLowRegister(), BC.getHighRegister());
    case 0x09:
        return ld_r_r(BC.getLowRegister(), BC.getLowRegister());
    case 0x0A:
        return ld_r_r(BC.getLowRegister(), DE.getHighRegister());
    case 0x0B:
        return ld_r_r(BC.getLowRegister(), DE.getLowRegister());
    case 0x0C:
        return ld_r_r(BC.getLowRegister(), HL.getHighRegister());
    case 0x0D:
        return ld_r_r(BC.getLowRegister(), HL.getLowRegister());
    case 0x0E:
        return ld_r_HL(BC.getLowRegister());
    case 0x0F:
        return ld_r_r(BC.getLowRegister(), AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode);
    }
}
int CPU::execute0x5XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return ld_r_r(DE.getHighRegister(), BC.getHighRegister());
    case 0x01:
        return ld_r_r(DE.getHighRegister(), BC.getLowRegister());
    case 0x02:
        return ld_r_r(DE.getHighRegister(), DE.getHighRegister());
    case 0x03:
        return ld_r_r(DE.getHighRegister(), DE.getLowRegister());
    case 0x04:
        return ld_r_r(DE.getHighRegister(), HL.getHighRegister());
    case 0x05:
        return ld_r_r(DE.getHighRegister(), HL.getLowRegister());
    case 0x06:
        return ld_r_HL(DE.getHighRegister());
    case 0x07:
        return ld_r_r(DE.getHighRegister(), AF.getHighRegister());
    case 0x08:
        return ld_r_r(DE.getLowRegister(), BC.getHighRegister());
    case 0x09:
        return ld_r_r(DE.getLowRegister(), BC.getLowRegister());
    case 0x0A:
        return ld_r_r(DE.getLowRegister(), DE.getHighRegister());
    case 0x0B:
        return ld_r_r(DE.getLowRegister(), DE.getLowRegister());
    case 0x0C:
        return ld_r_r(DE.getLowRegister(), HL.getHighRegister());
    case 0x0D:
        return ld_r_r(DE.getLowRegister(), HL.getLowRegister());
    case 0x0E:
        return ld_r_HL(DE.getLowRegister());
    case 0x0F:
        return ld_r_r(DE.getLowRegister(), AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0x6XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return ld_r_r(HL.getHighRegister(), BC.getHighRegister());
    case 0x01:
        return ld_r_r(HL.getHighRegister(), BC.getLowRegister());
    case 0x02:
        return ld_r_r(HL.getHighRegister(), DE.getHighRegister());
    case 0x03:
        return ld_r_r(HL.getHighRegister(), DE.getLowRegister());
    case 0x04:
        return ld_r_r(HL.getHighRegister(), HL.getHighRegister());
    case 0x05:
        return ld_r_r(HL.getHighRegister(), HL.getLowRegister());
    case 0x06:
        return ld_r_HL(HL.getHighRegister());
    case 0x07:
        return ld_r_r(HL.getHighRegister(), AF.getHighRegister());
    case 0x08:
        return ld_r_r(HL.getLowRegister(), BC.getHighRegister());
    case 0x09:
        return ld_r_r(HL.getLowRegister(), BC.getLowRegister());
    case 0x0A:
        return ld_r_r(HL.getLowRegister(), DE.getHighRegister());
    case 0x0B:
        return ld_r_r(HL.getLowRegister(), DE.getLowRegister());
    case 0x0C:
        return ld_r_r(HL.getLowRegister(), HL.getHighRegister());
    case 0x0D:
        return ld_r_r(HL.getLowRegister(), HL.getLowRegister());
    case 0x0E:
        return ld_r_HL(HL.getLowRegister());
    case 0x0F:
        return ld_r_r(HL.getLowRegister(), AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0x7XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return ld_HL_r(BC.getHighRegister());
    case 0x01:
        return ld_HL_r(BC.getLowRegister());
    case 0x02:
        return ld_HL_r(DE.getHighRegister());
    case 0x03:
        return ld_HL_r(DE.getLowRegister());
    case 0x04:
        return ld_HL_r(HL.getHighRegister());
    case 0x05:
        return ld_HL_r(HL.getLowRegister());
    case 0x06:
        return halt();
    case 0x07:
        return ld_HL_r(AF.getHighRegister());
    case 0x08:
        return ld_r_r(AF.getHighRegister(), BC.getHighRegister());
    case 0x09:
        return ld_r_r(AF.getHighRegister(), BC.getLowRegister());
    case 0x0A:
        return ld_r_r(AF.getHighRegister(), DE.getHighRegister());
    case 0x0B:
        return ld_r_r(AF.getHighRegister(), DE.getLowRegister());
    case 0x0C:
        return ld_r_r(AF.getHighRegister(), HL.getHighRegister());
    case 0x0D:
        return ld_r_r(AF.getHighRegister(), HL.getLowRegister());
    case 0x0E:
        return ld_r_HL(AF.getHighRegister());
    case 0x0F:
        return ld_r_r(AF.getHighRegister(), AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0x8XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return add_A_r(BC.getHighRegister());
    case 0x01:
        return add_A_r(BC.getLowRegister());
    case 0x02:
        return add_A_r(DE.getHighRegister());
    case 0x03:
        return add_A_r(DE.getLowRegister());
    case 0x04:
        return add_A_r(HL.getHighRegister());
    case 0x05:
        return add_A_r(HL.getLowRegister());
    case 0x06:
        return add_A_HL();
    case 0x07:
        return add_A_r(AF.getHighRegister());
    case 0x08:
        return adc_A_r(BC.getHighRegister());
    case 0x09:
        return adc_A_r(BC.getLowRegister());
    case 0x0A:
        return adc_A_r(DE.getHighRegister());
    case 0x0B:
        return adc_A_r(DE.getLowRegister());
    case 0x0C:
        return adc_A_r(HL.getHighRegister());
    case 0x0D:
        return adc_A_r(HL.getLowRegister());
    case 0x0E:
        return adc_A_HL();
    case 0x0F:
        return adc_A_r(AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0x9XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return sub_A_r(BC.getHighRegister());
    case 0x01:
        return sub_A_r(BC.getLowRegister());
    case 0x02:
        return sub_A_r(DE.getHighRegister());
    case 0x03:
        return sub_A_r(DE.getLowRegister());
    case 0x04:
        return sub_A_r(HL.getHighRegister());
    case 0x05:
        return sub_A_r(HL.getLowRegister());
    case 0x06:
        return sub_A_HL();
    case 0x07:
        return sub_A_r(AF.getHighRegister());
    case 0x08:
        return sbc_A_r(BC.getHighRegister());
    case 0x09:
        return sbc_A_r(BC.getLowRegister());
    case 0x0A:
        return sbc_A_r(DE.getHighRegister());
    case 0x0B:
        return sbc_A_r(DE.getLowRegister());
    case 0x0C:
        return sbc_A_r(HL.getHighRegister());
    case 0x0D:
        return sbc_A_r(HL.getLowRegister());
    case 0x0E:
        return sbc_A_HL();
    case 0x0F:
        return sbc_A_r(AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0xAXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return and_A_r(BC.getHighRegister());
    case 0x01:
        return and_A_r(BC.getLowRegister());
    case 0x02:
        return and_A_r(DE.getHighRegister());
    case 0x03:
        return and_A_r(DE.getLowRegister());
    case 0x04:
        return and_A_r(HL.getHighRegister());
    case 0x05:
        return and_A_r(HL.getLowRegister());
    case 0x06:
        return and_A_HL();
    case 0x07:
        return and_A_r(AF.getHighRegister());
    case 0x08:
        return xor_A_r(BC.getHighRegister());
    case 0x09:
        return xor_A_r(BC.getLowRegister());
    case 0x0A:
        return xor_A_r(DE.getHighRegister());
    case 0x0B:
        return xor_A_r(DE.getLowRegister());
    case 0x0C:
        return xor_A_r(HL.getHighRegister());
    case 0x0D:
        return xor_A_r(HL.getLowRegister());
    case 0x0E:
        return xor_A_HL();
    case 0x0F:
        return xor_A_r(AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode);
    }
}
int CPU::execute0xBXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return or_A_r(BC.getHighRegister());
    case 0x01:
        return or_A_r(BC.getLowRegister());
    case 0x02:
        return or_A_r(DE.getHighRegister());
    case 0x03:
        return or_A_r(DE.getLowRegister());
    case 0x04:
        return or_A_r(HL.getHighRegister());
    case 0x05:
        return or_A_r(HL.getLowRegister());
    case 0x06:
        return or_A_HL();
    case 0x07:
        return or_A_r(AF.getHighRegister());
    case 0x08:
        return cp_A_r(BC.getHighRegister());
    case 0x09:
        return cp_A_r(BC.getLowRegister());
    case 0x0A:
        return cp_A_r(DE.getHighRegister());
    case 0x0B:
        return cp_A_r(DE.getLowRegister());
    case 0x0C:
        return cp_A_r(HL.getHighRegister());
    case 0x0D:
        return cp_A_r(HL.getLowRegister());
    case 0x0E:
        return cp_A_HL();
    case 0x0F:
        return cp_A_r(AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0xCXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return ret_f(AF.getZeroFlag(), 0);
    case 0x01:
        return pop_rr(BC.getFullRegister());
    case 0x02:
        return jp_f_nn(AF.getZeroFlag(), 0);
    case 0x03:
        return jp_nn();
    case 0x04:
        return call_f_nn(AF.getZeroFlag(), 0);
    case 0x05:
        return push_rr(BC.getFullRegister());
    case 0x06:
        return add_A_n();
    case 0x07:
        return rst_n(0x00);
    case 0x08:
        return ret_f(AF.getZeroFlag(), 1);
    case 0x09:
        return ret();
    case 0x0A:
        return jp_f_nn(AF.getZeroFlag(), 1);
    case 0x0B: {
        uint8_t opcode = mmu.read(PC.getPCValue());
        PC.incrementPC(1);
        return execute0xCBOpcode(opcode);
    }
    case 0x0C:
        return call_f_nn(AF.getZeroFlag(), 1);
    case 0x0D:
        return call_nn();
    case 0x0E:
        return adc_A_n();
    case 0x0F:
        return rst_n(0x08);
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0xDXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return ret_f(AF.getCarryFlag(), 0);
    case 0x01:
        return pop_rr(DE.getFullRegister());
    case 0x02:
        return jp_f_nn(AF.getCarryFlag(), 0);
    case 0x04:
        return call_f_nn(AF.getCarryFlag(), 0);
    case 0x05:
        return push_rr(DE.getFullRegister());
    case 0x06:
        return sub_A_n();
    case 0x07:
        return rst_n(0x10);
    case 0x08:
        return ret_f(AF.getCarryFlag(), 1);
    case 0x09:
        return reti();
    case 0x0A:
        return jp_f_nn(AF.getCarryFlag(), 1);
    case 0x0C:
        return call_f_nn(AF.getCarryFlag(), 1);
    case 0x0E:
        return sbc_A_n();
    case 0x0F:
        return rst_n(0x18);
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0xEXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return ld_FFn_A();
    case 0x01:
        return pop_rr(HL.getFullRegister());
    case 0x02:
        return ld_FFC_A();
    case 0x05:
        return push_rr(HL.getFullRegister());
    case 0x06:
        return and_A_n();
    case 0x07:
        return rst_n(0x20);
    case 0x08:
        return add_SP_dd_relative();
    case 0x09:
        return jp_HL();
    case 0x0A:
        return ld_nn_A();
    case 0x0E:
        return xor_A_n();
    case 0x0F:
        return rst_n(0x28);
    default:
        return unsupportedOpcode(opcode);
    }
}

int CPU::execute0xFXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return ld_A_FFn();
    case 0x01:
        return pop_rr(AF.getFullRegister(), true);
    case 0x02:
        return ld_A_FFC();
    case 0x03:
        return di();
    case 0x05:
        return push_rr(AF.getFullRegister());
    case 0x06:
        return or_A_n();
    case 0x07:
        return rst_n(0x30);
    case 0x08:
        return ld_HL_SP_dd_relative();
    case 0x09:
        return ld_SP_HL();
    case 0x0A:
        return ld_A_nn();
    case 0x0B:
        return ei();
    case 0x0E:
        return cp_A_n();
    case 0x0F:
        return rst_n(0x38);
    default:
        return unsupportedOpcode(opcode);
    }
}
} // namespace GB
