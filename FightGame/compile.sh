#!/bin/bash

clang++ -std=c++1y -O2 -DNDEBUG -pthread -Wall -Wextra \
	-lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio \
	"${@:2}" ./$1 -o /tmp/$1.temp && /tmp/$1.temp