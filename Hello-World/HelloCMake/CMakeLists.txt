cmake_minimum_required (VERSION 3.5)

project (HelloWorld.so)
# Use default flags and trigger warnings at the cpp version pointed
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
# Set the sauce root
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB source_files "${source_dir}/*.cpp")

add_executable (HelloWorld.so ${source_files})
