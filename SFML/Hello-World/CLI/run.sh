#!/bin/bash
# Instructions from: <https://www.sfml-dev.org/tutorials/2.5/start-linux.php>
g++ -c main.cpp
g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
./sfml-app
