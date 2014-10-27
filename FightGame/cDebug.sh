#!/bin/bash

g++ -std=c++1y -O0 -g3 -pthread -Wall -Wextra \
	-lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio \
	"${@:2}" ./$1 -o /tmp/$1.temp && /tmp/$1.temp