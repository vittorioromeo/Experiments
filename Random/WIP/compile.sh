#!/bin/bash

clang++ -std=c++11 -O3 -DNDEBUG -pthread "${@:2}" ./$1 -o /tmp/$1.temp && /tmp/$1.temp