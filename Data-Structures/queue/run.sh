#!/bin/bash
make clear

debug="$1"
make "$debug"
./queue.out
