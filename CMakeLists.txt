cmake_minimum_required (VERSION 3.5)
project(Chip8)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -O3")

set (Chip8_VERSION_MAJOR 0)
set (Chip8_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Chip8Config.h.in"
  "${PROJECT_BINARY_DIR}/Chip8Config.h"
  )

# add the binary tree to the search path for include files so we'll find Chip8Config.h
include_directories("${PROJECT_BINARY_DIR}")

file(GLOB_RECURSE SOURCE_FILES "src/*.c")
add_executable(chip8 ${SOURCE_FILES})

# SDL Config
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8 ${SDL2_LIBRARIES})
