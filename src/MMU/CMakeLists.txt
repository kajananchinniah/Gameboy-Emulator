set(MMU_SOURCES
    MMU.cpp
    MMU_banks.cpp
    MMU_cpu_interrupts_and_timers.cpp
    MMU_joypad.cpp
    MMU_MBC0.cpp
    MMU_MBC1.cpp
    MMU_MBC3.cpp
    MMU_ppu_registers.cpp)

add_library(mmu ${MMU_SOURCES})
