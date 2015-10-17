#!/bin/zsh

sudo rm -R /usr/local/include/vrm/sdl/*
sudo cp -R ./vrm/ /usr/local/include/
mkdir build

clang++ -g3 -fno-omit-frame-pointer -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx -lGL -lGLEW -lglut \
    -std=c++14 "${@:2}" \
    -o ./build/"$1".x ./"$1".cpp \
&& vblank_mode=0 __GL_SYNC_TO_VBLANK=0 ./build/"$1".x 

#    -O3 -ffast-math \
#    -pthread -Wall -Wextra -Wpedantic -Wundef -Wno-missing-field-initializers \
#    -Wpointer-arith -Wcast-align -Wwrite-strings -Wno-unreachable-code -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC \

