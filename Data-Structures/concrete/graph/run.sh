#!/bin/bash
make

if [[ $? == 0 ]] ; then
	../.bin/graph.out
else
	>&2 printf "\33[31mError [$?]\33[m\n"
fi
