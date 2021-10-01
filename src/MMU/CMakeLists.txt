add_library(mmu MMU.cpp MMU_cpu_interrupts_and_timers.cpp)
target_include_directories(mmu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
