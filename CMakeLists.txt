cmake_minimum_required(VERSION 3.10)
project(MathOperations)

set(CMAKE_CXX_STANDARD 17)

add_executable(MathOperations scr/main.cpp scr/operation_generator.h)