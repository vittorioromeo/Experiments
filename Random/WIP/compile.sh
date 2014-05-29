#!/bin/bash

clang++ -std=c++11 -O3 -DNDEBUG ./$1 -o $1 && ./$1