#!/bin/zsh

sudo cp -R ./vrm/ /usr/local/include/

clang++ -lSDL2 -lSDL2_ttf -lSDL2_image -lGL -lGLEW \
    -std=c++14 "${@:2}" \
    -o "$1".x ./"$1".cpp \
&& ./"$1".x
#    -O3 -ffast-math \

