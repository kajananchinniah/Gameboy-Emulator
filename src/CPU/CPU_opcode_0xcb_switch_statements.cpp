#include "GB/CPU.hpp"

namespace GB
{
int CPU::execute0xCBOpcode(uint8_t opcode)
{
    switch (opcode & 0xF0) {
    case 0x00:
        return execute0xCB0XTable(opcode);
    case 0x10:
        return execute0xCB1XTable(opcode);
    case 0x20:
        return execute0xCB2XTable(opcode);
    case 0x30:
        return execute0xCB3XTable(opcode);
    case 0x40:
        return execute0xCB4XTable(opcode);
    case 0x50:
        return execute0xCB5XTable(opcode);
    case 0x60:
        return execute0xCB6XTable(opcode);
    case 0x70:
        return execute0xCB7XTable(opcode);
    case 0x80:
        return execute0xCB8XTable(opcode);
    case 0x90:
        return execute0xCB9XTable(opcode);
    case 0xA0:
        return execute0xCBAXTable(opcode);
    case 0xB0:
        return execute0xCBBXTable(opcode);
    case 0xC0:
        return execute0xCBCXTable(opcode);
    case 0xD0:
        return execute0xCBDXTable(opcode);
    case 0xE0:
        return execute0xCBEXTable(opcode);
    case 0xF0:
        return execute0xCBFXTable(opcode);
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCB0XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return rlc_r(BC.getHighRegister());
    case 0x01:
        return rlc_r(BC.getLowRegister());
    case 0x02:
        return rlc_r(DE.getHighRegister());
    case 0x03:
        return rlc_r(DE.getLowRegister());
    case 0x04:
        return rlc_r(HL.getHighRegister());
    case 0x05:
        return rlc_r(HL.getLowRegister());
    case 0x06:
        return rlc_HL();
    case 0x07:
        return rlc_r(AF.getHighRegister());
    case 0x08:
        return rrc_r(BC.getHighRegister());
    case 0x09:
        return rrc_r(BC.getLowRegister());
    case 0x0A:
        return rrc_r(DE.getHighRegister());
    case 0x0B:
        return rrc_r(DE.getLowRegister());
    case 0x0C:
        return rrc_r(HL.getHighRegister());
    case 0x0D:
        return rrc_r(HL.getLowRegister());
    case 0x0E:
        return rrc_HL();
    case 0x0F:
        return rrc_r(AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}
int CPU::execute0xCB1XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return rl_r(BC.getHighRegister());
    case 0x01:
        return rl_r(BC.getLowRegister());
    case 0x02:
        return rl_r(DE.getHighRegister());
    case 0x03:
        return rl_r(DE.getLowRegister());
    case 0x04:
        return rl_r(HL.getHighRegister());
    case 0x05:
        return rl_r(HL.getLowRegister());
    case 0x06:
        return rl_HL();
    case 0x07:
        return rl_r(AF.getHighRegister());
    case 0x08:
        return rr_r(BC.getHighRegister());
    case 0x09:
        return rr_r(BC.getLowRegister());
    case 0x0A:
        return rr_r(DE.getHighRegister());
    case 0x0B:
        return rr_r(DE.getLowRegister());
    case 0x0C:
        return rr_r(HL.getHighRegister());
    case 0x0D:
        return rr_r(HL.getLowRegister());
    case 0x0E:
        return rr_HL();
    case 0x0F:
        return rr_r(AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCB2XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return sla_r(BC.getHighRegister());
    case 0x01:
        return sla_r(BC.getLowRegister());
    case 0x02:
        return sla_r(DE.getHighRegister());
    case 0x03:
        return sla_r(DE.getLowRegister());
    case 0x04:
        return sla_r(HL.getHighRegister());
    case 0x05:
        return sla_r(HL.getLowRegister());
    case 0x06:
        return sla_HL();
    case 0x07:
        return sla_r(AF.getHighRegister());
    case 0x08:
        return sra_r(BC.getHighRegister());
    case 0x09:
        return sra_r(BC.getLowRegister());
    case 0x0A:
        return sra_r(DE.getHighRegister());
    case 0x0B:
        return sra_r(DE.getLowRegister());
    case 0x0C:
        return sra_r(HL.getHighRegister());
    case 0x0D:
        return sra_r(HL.getLowRegister());
    case 0x0E:
        return sra_HL();
    case 0x0F:
        return sra_r(AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCB3XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return swap_r(BC.getHighRegister());
    case 0x01:
        return swap_r(BC.getLowRegister());
    case 0x02:
        return swap_r(DE.getHighRegister());
    case 0x03:
        return swap_r(DE.getLowRegister());
    case 0x04:
        return swap_r(HL.getHighRegister());
    case 0x05:
        return swap_r(HL.getLowRegister());
    case 0x06:
        return swap_HL();
    case 0x07:
        return swap_r(AF.getHighRegister());
    case 0x08:
        return srl_r(BC.getHighRegister());
    case 0x09:
        return srl_r(BC.getLowRegister());
    case 0x0A:
        return srl_r(DE.getHighRegister());
    case 0x0B:
        return srl_r(DE.getLowRegister());
    case 0x0C:
        return srl_r(HL.getHighRegister());
    case 0x0D:
        return srl_r(HL.getLowRegister());
    case 0x0E:
        return srl_HL();
    case 0x0F:
        return srl_r(AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCB4XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return bit_n_r(0, BC.getHighRegister());
    case 0x01:
        return bit_n_r(0, BC.getLowRegister());
    case 0x02:
        return bit_n_r(0, DE.getHighRegister());
    case 0x03:
        return bit_n_r(0, DE.getLowRegister());
    case 0x04:
        return bit_n_r(0, HL.getHighRegister());
    case 0x05:
        return bit_n_r(0, HL.getLowRegister());
    case 0x06:
        return bit_n_HL(0);
    case 0x07:
        return bit_n_r(0, AF.getHighRegister());
    case 0x08:
        return bit_n_r(1, BC.getHighRegister());
    case 0x09:
        return bit_n_r(1, BC.getLowRegister());
    case 0x0A:
        return bit_n_r(1, DE.getHighRegister());
    case 0x0B:
        return bit_n_r(1, DE.getLowRegister());
    case 0x0C:
        return bit_n_r(1, HL.getHighRegister());
    case 0x0D:
        return bit_n_r(1, HL.getLowRegister());
    case 0x0E:
        return bit_n_HL(1);
    case 0x0F:
        return bit_n_r(1, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCB5XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return bit_n_r(2, BC.getHighRegister());
    case 0x01:
        return bit_n_r(2, BC.getLowRegister());
    case 0x02:
        return bit_n_r(2, DE.getHighRegister());
    case 0x03:
        return bit_n_r(2, DE.getLowRegister());
    case 0x04:
        return bit_n_r(2, HL.getHighRegister());
    case 0x05:
        return bit_n_r(2, HL.getLowRegister());
    case 0x06:
        return bit_n_HL(2);
    case 0x07:
        return bit_n_r(2, AF.getHighRegister());
    case 0x08:
        return bit_n_r(3, BC.getHighRegister());
    case 0x09:
        return bit_n_r(3, BC.getLowRegister());
    case 0x0A:
        return bit_n_r(3, DE.getHighRegister());
    case 0x0B:
        return bit_n_r(3, DE.getLowRegister());
    case 0x0C:
        return bit_n_r(3, HL.getHighRegister());
    case 0x0D:
        return bit_n_r(3, HL.getLowRegister());
    case 0x0E:
        return bit_n_HL(3);
    case 0x0F:
        return bit_n_r(3, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCB6XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return bit_n_r(4, BC.getHighRegister());
    case 0x01:
        return bit_n_r(4, BC.getLowRegister());
    case 0x02:
        return bit_n_r(4, DE.getHighRegister());
    case 0x03:
        return bit_n_r(4, DE.getLowRegister());
    case 0x04:
        return bit_n_r(4, HL.getHighRegister());
    case 0x05:
        return bit_n_r(4, HL.getLowRegister());
    case 0x06:
        return bit_n_HL(4);
    case 0x07:
        return bit_n_r(4, AF.getHighRegister());
    case 0x08:
        return bit_n_r(5, BC.getHighRegister());
    case 0x09:
        return bit_n_r(5, BC.getLowRegister());
    case 0x0A:
        return bit_n_r(5, DE.getHighRegister());
    case 0x0B:
        return bit_n_r(5, DE.getLowRegister());
    case 0x0C:
        return bit_n_r(5, HL.getHighRegister());
    case 0x0D:
        return bit_n_r(5, HL.getLowRegister());
    case 0x0E:
        return bit_n_HL(5);
    case 0x0F:
        return bit_n_r(5, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCB7XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return bit_n_r(6, BC.getHighRegister());
    case 0x01:
        return bit_n_r(6, BC.getLowRegister());
    case 0x02:
        return bit_n_r(6, DE.getHighRegister());
    case 0x03:
        return bit_n_r(6, DE.getLowRegister());
    case 0x04:
        return bit_n_r(6, HL.getHighRegister());
    case 0x05:
        return bit_n_r(6, HL.getLowRegister());
    case 0x06:
        return bit_n_HL(6);
    case 0x07:
        return bit_n_r(6, AF.getHighRegister());
    case 0x08:
        return bit_n_r(7, BC.getHighRegister());
    case 0x09:
        return bit_n_r(7, BC.getLowRegister());
    case 0x0A:
        return bit_n_r(7, DE.getHighRegister());
    case 0x0B:
        return bit_n_r(7, DE.getLowRegister());
    case 0x0C:
        return bit_n_r(7, HL.getHighRegister());
    case 0x0D:
        return bit_n_r(7, HL.getLowRegister());
    case 0x0E:
        return bit_n_HL(7);
    case 0x0F:
        return bit_n_r(7, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCB8XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return res_n_r(0, BC.getHighRegister());
    case 0x01:
        return res_n_r(0, BC.getLowRegister());
    case 0x02:
        return res_n_r(0, DE.getHighRegister());
    case 0x03:
        return res_n_r(0, DE.getLowRegister());
    case 0x04:
        return res_n_r(0, HL.getHighRegister());
    case 0x05:
        return res_n_r(0, HL.getLowRegister());
    case 0x06:
        return res_n_HL(0);
    case 0x07:
        return res_n_r(0, AF.getHighRegister());
    case 0x08:
        return res_n_r(1, BC.getHighRegister());
    case 0x09:
        return res_n_r(1, BC.getLowRegister());
    case 0x0A:
        return res_n_r(1, DE.getHighRegister());
    case 0x0B:
        return res_n_r(1, DE.getLowRegister());
    case 0x0C:
        return res_n_r(1, HL.getHighRegister());
    case 0x0D:
        return res_n_r(1, HL.getLowRegister());
    case 0x0E:
        return res_n_HL(1);
    case 0x0F:
        return res_n_r(1, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCB9XTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return res_n_r(2, BC.getHighRegister());
    case 0x01:
        return res_n_r(2, BC.getLowRegister());
    case 0x02:
        return res_n_r(2, DE.getHighRegister());
    case 0x03:
        return res_n_r(2, DE.getLowRegister());
    case 0x04:
        return res_n_r(2, HL.getHighRegister());
    case 0x05:
        return res_n_r(2, HL.getLowRegister());
    case 0x06:
        return res_n_HL(2);
    case 0x07:
        return res_n_r(2, AF.getHighRegister());
    case 0x08:
        return res_n_r(3, BC.getHighRegister());
    case 0x09:
        return res_n_r(3, BC.getLowRegister());
    case 0x0A:
        return res_n_r(3, DE.getHighRegister());
    case 0x0B:
        return res_n_r(3, DE.getLowRegister());
    case 0x0C:
        return res_n_r(3, HL.getHighRegister());
    case 0x0D:
        return res_n_r(3, HL.getLowRegister());
    case 0x0E:
        return res_n_HL(3);
    case 0x0F:
        return res_n_r(3, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCBAXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return res_n_r(4, BC.getHighRegister());
    case 0x01:
        return res_n_r(4, BC.getLowRegister());
    case 0x02:
        return res_n_r(4, DE.getHighRegister());
    case 0x03:
        return res_n_r(4, DE.getLowRegister());
    case 0x04:
        return res_n_r(4, HL.getHighRegister());
    case 0x05:
        return res_n_r(4, HL.getLowRegister());
    case 0x06:
        return res_n_HL(4);
    case 0x07:
        return res_n_r(4, AF.getHighRegister());
    case 0x08:
        return res_n_r(5, BC.getHighRegister());
    case 0x09:
        return res_n_r(5, BC.getLowRegister());
    case 0x0A:
        return res_n_r(5, DE.getHighRegister());
    case 0x0B:
        return res_n_r(5, DE.getLowRegister());
    case 0x0C:
        return res_n_r(5, HL.getHighRegister());
    case 0x0D:
        return res_n_r(5, HL.getLowRegister());
    case 0x0E:
        return res_n_HL(5);
    case 0x0F:
        return res_n_r(5, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCBBXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return res_n_r(6, BC.getHighRegister());
    case 0x01:
        return res_n_r(6, BC.getLowRegister());
    case 0x02:
        return res_n_r(6, DE.getHighRegister());
    case 0x03:
        return res_n_r(6, DE.getLowRegister());
    case 0x04:
        return res_n_r(6, HL.getHighRegister());
    case 0x05:
        return res_n_r(6, HL.getLowRegister());
    case 0x06:
        return res_n_HL(6);
    case 0x07:
        return res_n_r(6, AF.getHighRegister());
    case 0x08:
        return res_n_r(7, BC.getHighRegister());
    case 0x09:
        return res_n_r(7, BC.getLowRegister());
    case 0x0A:
        return res_n_r(7, DE.getHighRegister());
    case 0x0B:
        return res_n_r(7, DE.getLowRegister());
    case 0x0C:
        return res_n_r(7, HL.getHighRegister());
    case 0x0D:
        return res_n_r(7, HL.getLowRegister());
    case 0x0E:
        return res_n_HL(7);
    case 0x0F:
        return res_n_r(7, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCBCXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return set_n_r(0, BC.getHighRegister());
    case 0x01:
        return set_n_r(0, BC.getLowRegister());
    case 0x02:
        return set_n_r(0, DE.getHighRegister());
    case 0x03:
        return set_n_r(0, DE.getLowRegister());
    case 0x04:
        return set_n_r(0, HL.getHighRegister());
    case 0x05:
        return set_n_r(0, HL.getLowRegister());
    case 0x06:
        return set_n_HL(0);
    case 0x07:
        return set_n_r(0, AF.getHighRegister());
    case 0x08:
        return set_n_r(1, BC.getHighRegister());
    case 0x09:
        return set_n_r(1, BC.getLowRegister());
    case 0x0A:
        return set_n_r(1, DE.getHighRegister());
    case 0x0B:
        return set_n_r(1, DE.getLowRegister());
    case 0x0C:
        return set_n_r(1, HL.getHighRegister());
    case 0x0D:
        return set_n_r(1, HL.getLowRegister());
    case 0x0E:
        return set_n_HL(1);
    case 0x0F:
        return set_n_r(1, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCBDXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return set_n_r(2, BC.getHighRegister());
    case 0x01:
        return set_n_r(2, BC.getLowRegister());
    case 0x02:
        return set_n_r(2, DE.getHighRegister());
    case 0x03:
        return set_n_r(2, DE.getLowRegister());
    case 0x04:
        return set_n_r(2, HL.getHighRegister());
    case 0x05:
        return set_n_r(2, HL.getLowRegister());
    case 0x06:
        return set_n_HL(2);
    case 0x07:
        return set_n_r(2, AF.getHighRegister());
    case 0x08:
        return set_n_r(3, BC.getHighRegister());
    case 0x09:
        return set_n_r(3, BC.getLowRegister());
    case 0x0A:
        return set_n_r(3, DE.getHighRegister());
    case 0x0B:
        return set_n_r(3, DE.getLowRegister());
    case 0x0C:
        return set_n_r(3, HL.getHighRegister());
    case 0x0D:
        return set_n_r(3, HL.getLowRegister());
    case 0x0E:
        return set_n_HL(3);
    case 0x0F:
        return set_n_r(3, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCBEXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return set_n_r(4, BC.getHighRegister());
    case 0x01:
        return set_n_r(4, BC.getLowRegister());
    case 0x02:
        return set_n_r(4, DE.getHighRegister());
    case 0x03:
        return set_n_r(4, DE.getLowRegister());
    case 0x04:
        return set_n_r(4, HL.getHighRegister());
    case 0x05:
        return set_n_r(4, HL.getLowRegister());
    case 0x06:
        return set_n_HL(4);
    case 0x07:
        return set_n_r(4, AF.getHighRegister());
    case 0x08:
        return set_n_r(5, BC.getHighRegister());
    case 0x09:
        return set_n_r(5, BC.getLowRegister());
    case 0x0A:
        return set_n_r(5, DE.getHighRegister());
    case 0x0B:
        return set_n_r(5, DE.getLowRegister());
    case 0x0C:
        return set_n_r(5, HL.getHighRegister());
    case 0x0D:
        return set_n_r(5, HL.getLowRegister());
    case 0x0E:
        return set_n_HL(5);
    case 0x0F:
        return set_n_r(5, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}

int CPU::execute0xCBFXTable(uint8_t opcode)
{
    switch (opcode & 0x0F) {
    case 0x00:
        return set_n_r(6, BC.getHighRegister());
    case 0x01:
        return set_n_r(6, BC.getLowRegister());
    case 0x02:
        return set_n_r(6, DE.getHighRegister());
    case 0x03:
        return set_n_r(6, DE.getLowRegister());
    case 0x04:
        return set_n_r(6, HL.getHighRegister());
    case 0x05:
        return set_n_r(6, HL.getLowRegister());
    case 0x06:
        return set_n_HL(6);
    case 0x07:
        return set_n_r(6, AF.getHighRegister());
    case 0x08:
        return set_n_r(7, BC.getHighRegister());
    case 0x09:
        return set_n_r(7, BC.getLowRegister());
    case 0x0A:
        return set_n_r(7, DE.getHighRegister());
    case 0x0B:
        return set_n_r(7, DE.getLowRegister());
    case 0x0C:
        return set_n_r(7, HL.getHighRegister());
    case 0x0D:
        return set_n_r(7, HL.getLowRegister());
    case 0x0E:
        return set_n_HL(7);
    case 0x0F:
        return set_n_r(7, AF.getHighRegister());
    default:
        return unsupportedOpcode(opcode, "cb");
    }
}
} // namespace GB
