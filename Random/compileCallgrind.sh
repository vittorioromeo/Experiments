#!/bin/bash

K=0

function finish 
{
	if [ $K -eq 1 ]; then
		qcachegrind ./callgrind.out*
	fi
}

trap finish EXIT

g++ -std=c++1y -O3 -g3 -DNDEBUG -pthread "${@:2}" ./$1 -o /tmp/$1.temp && \
rm ./callgrind.out*; K=1; valgrind --tool=callgrind /tmp/$1.temp 