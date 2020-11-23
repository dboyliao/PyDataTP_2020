cmake_minimum_required(VERSION 3.10)
project(PyDataTP2020)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti")

add_subdirectory(submodules/uTensor/src)
add_subdirectory(hello_world)
add_subdirectory(error_handling)