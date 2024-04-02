#!/bin/bash

for cpp in *.cpp; do
    g++ -c "$cpp"
done
g++ *.o -o test.so
./test.so

