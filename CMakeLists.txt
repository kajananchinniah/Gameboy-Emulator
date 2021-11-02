cmake_minimum_required(VERSION 3.10)
project(GB_Emulator LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

## Find packages ##
find_package(GTest REQUIRED)
find_package(SDL2 REQUIRED)

## Include CTest ##
include(CTest)

## Include directories ##
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS})

## Create libraries ##
AUX_SOURCE_DIRECTORY(src/Registers register_sources)
add_library(registers ${register_sources})

AUX_SOURCE_DIRECTORY(src/MMU mmu_sources)
add_library(mmu ${mmu_sources})

AUX_SOURCE_DIRECTORY(src/PPU ppu_sources)
add_library(ppu ${ppu_sources})
target_link_libraries(ppu mmu)

AUX_SOURCE_DIRECTORY(src/SDL gui_sources)
add_library(gui ${gui_sources})
target_link_libraries(gui ${SDL2_LIBRARIES})

AUX_SOURCE_DIRECTORY(src/CPU cpu_sources)
add_library(cpu ${cpu_sources})
target_link_libraries(cpu registers mmu ppu gui)

add_executable(main src/main.cpp)
target_link_libraries(main cpu)

## Testing ##
# Registers test
add_executable(test_registers tests/test_registers.cpp)
target_link_libraries(test_registers registers ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestRegisters COMMAND test_registers)
