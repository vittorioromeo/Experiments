#!/bin/bash

g++ -std=c++1y -O0 -g3 -pthread "${@:2}" ./$1 -o /tmp/$1.temp && ddd /tmp/$1.temp