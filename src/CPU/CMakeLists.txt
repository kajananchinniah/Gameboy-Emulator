set(CPU_SOURCES
    CPU.cpp
    CPU_16bit_arthimetic_instrs.cpp
    CPU_16bit_load_instrs.cpp
    CPU_8bit_arthimetic_instrs.cpp
    CPU_8bit_load_instrs.cpp
    CPU_control_instrs.cpp
    CPU_jump_instrs.cpp
    CPU_opcode_0xcb_switch_statements.cpp
    CPU_opcode_switch_statements.cpp
    CPU_rotate_and_shift_instrs.cpp
    CPU_single_bit_instrs.cpp)

add_library(cpu ${CPU_SOURCES})
target_link_libraries(cpu PUBLIC registers mmu ppu)
target_link_libraries(cpu PRIVATE gui)
