cmake_minimum_required(VERSION 3.17)
project(chip8emulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(chip8emulator main.cpp Chip8State.h Chip8.cpp Chip8.h Chip8Disassembler.cpp Chip8Disassembler.h types.h fileutil.h fileutil.cpp)