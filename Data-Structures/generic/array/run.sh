#!/bin/bash
make

if [[ $? == 0 ]] ; then
	./array.out
fi
