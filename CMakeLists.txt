cmake_minimum_required (VERSION 3.5)
project(Chip8)

set (Chip8_VERSION_MAJOR 0)
set (Chip8_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Chip8Config.h.in"
  "${PROJECT_BINARY_DIR}/Chip8Config.h"
  )

# add the binary tree to the search path for include files so we'll find Chip8Config.h
include_directories("${PROJECT_BINARY_DIR}")

add_executable(Chip8 src/main.c)
