#!/bin/zsh

sudo rm -R /usr/local/include/vrm/sdl/*
sudo cp -R ./vrm/ /usr/local/include/

g++ -g3 -fno-omit-frame-pointer -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx -lGL -lGLEW -lglut \
    -std=c++14 "${@:2}" \
    -o "$1".x ./"$1".cpp \
&& vblank_mode=0 __GL_SYNC_TO_VBLANK=0 ./"$1".x
#    -O3 -ffast-math \

