set(PPU_SOURCES PPU.cpp PPU_background.cpp PPU_sprites.cpp PPU_window.cpp)
add_library(ppu ${PPU_SOURCES})
target_link_libraries(ppu PUBLIC mmu)
