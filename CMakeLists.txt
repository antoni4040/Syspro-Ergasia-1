cmake_minimum_required(VERSION 3.13)
project(Ergasia_1 C)

set(CMAKE_C_STANDARD 11)

include_directories( ${MY_SOURCE_DIR})

add_executable(Ergasia_1 main.c verification.c structures.c)