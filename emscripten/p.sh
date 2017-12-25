#!/bin/zsh

g++ -g3 -fno-omit-frame-pointer -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx -lGL -lGLEW -lglut \
    -Iinclude \
    -I/home/vittorioromeo/OHWorkspace/vrm_pp/include \
    -I/home/vittorioromeo/OHWorkspace/vrm_core/include \
    -pedantic \
    -W \
    -Wall \
    -Wextra \
    -Wno-unused-local-typedefs \
    -Wwrite-strings \
    -Wundef \
    -Wno-missing-field-initializers \
    -Wpointer-arith \
    -Wcast-align \
    -Wno-unreachable-code \
    -Wnon-virtual-dtor \
    -Woverloaded-virtual \
    -Wsuggest-final-types \
    -Wsuggest-final-methods \
    -Wsuggest-override \
    -Wsequence-point \
    -std=c++14 "${@:2}" \
    -o ./build/"$1".x ./src/"$1".cpp \
&& vblank_mode=0 __GL_SYNC_TO_VBLANK=0 ./build/"$1".x

#    -O3 -ffast-math \
#    -pthread -Wall -Wextra -Wpedantic -Wundef -Wno-missing-field-initializers \
#    -Wpointer-arith -Wcast-align -Wwrite-strings -Wno-unreachable-code -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC \

