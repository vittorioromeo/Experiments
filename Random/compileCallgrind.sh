#!/bin/bash

clang++ -std=c++1y -O3 -DNDEBUG -pthread "${@:2}" ./$1 -o /tmp/$1.temp && \
rm ./callgrind.out*; valgrind --tool=callgrind /tmp/$1.temp ; \
qcachegrind ./callgrind.out*