#!/bin/bash

clang++ -std=c++1z -O0 -pthread -Wall -Wextra -Wpedantic -Wundef -Wshadow -Wno-missing-field-initializers \
	-Wpointer-arith -Wcast-align -Wwrite-strings -Wno-unreachable-code -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC \
	-I"/home/jackie/code/ecst/include/" \
	-I"/home/jackie/code/vrm_pp/include/" \
	-I"/home/jackie/code/vrm_core/include/" \
	-I"/home/jackie/code/metaprogramming/hana/include" \
	"${@:2}" ./$1
