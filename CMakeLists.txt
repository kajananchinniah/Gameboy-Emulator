cmake_minimum_required(VERSION 3.10)
project(GB_Emulator LANGUAGES CXX C)

## Find packages ##
find_package(GTest REQUIRED)

## Include CTest ##
include(CTest)

## Include directories ##
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)

## Create libraries ##
add_library(registers src/Registers.cpp)
add_library(mmu src/MMU.cpp)

AUX_SOURCE_DIRECTORY(src/CPU cpu_sources)
add_library(cpu ${cpu_sources})
target_link_libraries(cpu registers mmu)

add_executable(main src/main.cpp)
target_link_libraries(main cpu)

## Testing ##
# Registers test
add_executable(test_registers tests/test_registers.cpp)
target_link_libraries(test_registers registers ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestRegisters COMMAND test_registers)

# MMU test
add_executable(test_mmu tests/test_mmu.cpp)
target_link_libraries(test_mmu mmu ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestMMU COMMAND test_mmu)

# CPU test
add_executable(test_cpu tests/test_cpu.cpp)
target_link_libraries(test_cpu cpu ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestCPU COMMAND test_cpu)
