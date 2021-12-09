cmake_minimum_required(VERSION 3.10)
project(GB_Emulator LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(CTest)

# Include directories (global for simplicity) ##
include_directories(include)
add_subdirectory(src)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
