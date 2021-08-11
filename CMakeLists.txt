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

## Testing ##
add_executable(test_registers tests/test_registers.cpp)
target_link_libraries(test_registers registers ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME TestRegisters COMMAND test_registers)
