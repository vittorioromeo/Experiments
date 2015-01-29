#!/bin/bash

clang++ -std=c++1y -O0 -pthread -Wall -Wextra "${@:2}" ./$1 -o /tmp/$1.temp && /tmp/$1.temp