cmake_minimum_required(VERSION 3.10)
project(GB_Emulator LANGUAGES CXX C)

# Include directory
include_directories(include)

# Create libraries
add_library(registers src/Registers.cpp)

# Create executable
add_executable(main src/main.cpp)
target_link_libraries(main registers)
