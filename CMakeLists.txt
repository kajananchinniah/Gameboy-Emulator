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

## Testing ##
# Registers test
add_executable(test_registers tests/test_registers.cpp)
target_link_libraries(test_registers registers ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestRegisters COMMAND test_registers)

# MMU test
add_executable(test_mmu tests/test_mmu.cpp)
target_link_libraries(test_mmu mmu ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestMMU COMMAND test_mmu)

# Load opcode test
add_executable(test_opcodes_load tests/test_opcodes_load.cpp)
target_link_libraries(test_opcodes_load registers mmu ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestOpcodesLoad COMMAND test_opcodes_load)
