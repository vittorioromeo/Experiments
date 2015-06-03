#!/bin/bash

clang++ -std=c++1y -O3 -pthread -Wall -Wextra -Wpedantic -Wundef -Wshadow -Wno-missing-field-initializers \
	-Wpointer-arith -Wcast-align -Wwrite-strings -Wno-unreachable-code \
	-lsfml-system -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-window \
	"${@:2}" ./$1 -o /tmp/$1.temp && /tmp/$1.temp