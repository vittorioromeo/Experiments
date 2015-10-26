#!/bin/bash

# rm -R /usr/local/include/vrm/sdl/*
# cp -R ./vrm/ /usr/local/include/
# mkdir build

SDLDIR="D:\OHWorkspace\sdl2\i686-w64-mingw32"

g++ -g3 -fno-omit-frame-pointer -lSDL2 -lSDL2_ttf -lSDL2_image -lopengl32 -lglew32 -lglut -lglut32 \
    -std=c++14 "${@:2}" \
    -o ./build/"$1".x ./"$1".cpp \
    -I"." -I"D:/OHWorkspace/vrm_core/include" -I"D:/OHWorkspace/vrm_pp/include" \
    -I"$SDLDIR\include" -I"D:\OHWorkspace\glm" \
    -L"$SDLDIR\bin" \
&& vblank_mode=0 __GL_SYNC_TO_VBLANK=0 ./build/"$1".x 

#    -O3 -ffast-math \
#    -pthread -Wall -Wextra -Wpedantic -Wundef -Wno-missing-field-initializers \
#    -Wpointer-arith -Wcast-align -Wwrite-strings -Wno-unreachable-code -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC \

