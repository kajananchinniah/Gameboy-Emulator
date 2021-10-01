add_library(cpu CPU.cpp CPU_16bit_arthimetic_instrs.cpp CPU_16bit_load_instrs.cpp CPU_8bit_arthimetic_instrs.cpp
  CPU_8bit_load_instrs.cpp CPU_control_instrs.cpp CPU_jump_instrs.cpp CPU_opcode_0xcb_switch_statements.cpp
  CPU_opcode_switch_statements.cpp CPU_rotate_and_shift_instrs.cpp CPU_single_bit_instrs.cpp)

target_include_directories(cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cpu registers mmu)
