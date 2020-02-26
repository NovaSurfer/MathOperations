cmake_minimum_required(VERSION 3.10)
project(mathcore)

set(CMAKE_CXX_STANDARD 17)

# Lib
set(MATHCORE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(mathcore INTERFACE)
target_include_directories(mathcore INTERFACE ${MATHCORE_INCLUDE})

# Test executable
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests mathcore)




