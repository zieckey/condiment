cmake_minimum_required(VERSION 2.8)
project (HELLO)
message(STATUS "This is BINARY dir " ${CMAKE_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${CMAKE_SOURCE_DIR})
add_subdirectory(lib)
add_subdirectory(src bin)

include(CTest)
