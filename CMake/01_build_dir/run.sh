#!/bin/bash
cd ./build || exit 1
# Build generating compile_commands.json to use in debugging
# cmake cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../ && make && ./hello.so
cmake -DCMAKE_BUILD_TYPE="Debug" ../ && make && ./hello.so
cd ..
