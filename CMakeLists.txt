cmake_minimum_required(VERSION 3.10)
project(GB_Emulator LANGUAGES CXX C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

## Find packages ##
find_package(GTest REQUIRED)

## Include CTest ##
include(CTest)

## Include directories ##
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)

## Add subdirectories ##
add_subdirectory(src/Registers)
add_subdirectory(src/MMU)
add_subdirectory(src/CPU)
add_subdirectory(src)
add_subdirectory(tests)
