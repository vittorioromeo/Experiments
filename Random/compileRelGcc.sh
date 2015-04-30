#!/bin/bash

g++ -std=c++1y -O3 -g3 -DNDEBUG -pthread -Wall -Wextra "${@:2}" ./$1 -o /tmp/$1.temp && /tmp/$1.temp