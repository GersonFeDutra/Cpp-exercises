# CMake Hello World

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(hello.so VERSION 1.0)
add_executable(hello.so main.cpp)
