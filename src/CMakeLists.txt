add_subdirectory(Registers)
add_subdirectory(MMU)
add_subdirectory(PPU)
add_subdirectory(SDL)
add_subdirectory(CPU)

set(MAIN_SOURCES main.cpp)

add_executable(gb-emulator ${MAIN_SOURCES})
target_link_libraries(gb-emulator PRIVATE cpu)
