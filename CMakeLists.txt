# root file, branches to all modules
cmake_minimum_required(VERSION 3.10)

SET(EXE blc)

project(Bite C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

include(src/CMakeLists.txt)

include(src/parser/CMakeLists.txt)
include(src/cli/CMakeLists.txt)
include(src/builder/CMakeLists.txt)
