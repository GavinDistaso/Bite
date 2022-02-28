# root file, branches to all modules
cmake_minimum_required(VERSION 3.10)

SET(EXE cal)

project(ALang C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

include(src/CMakeLists.txt)

include(src/builder/CMakeLists.txt)
include(src/cli/CMakeLists.txt)

