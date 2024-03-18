cmake_minimum_required(VERSION 3.10)
project(Loom API)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.asm")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")