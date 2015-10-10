#!/bin/zsh

sudo cp -R ./vrm/ /usr/local/include/

clang++ -lSDL2 -lSDL2_ttf -lSDL2_image -lGL -lGLEW \
    -std=c++14 "$@" \
    -o main.x ./main.cpp \
&& ./main.x
#    -O3 -ffast-math \

