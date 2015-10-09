#!/bin/zsh

/usr/lib/emscripten/em++ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_LIBPNG=1 -s DEMANGLE_SUPPORT=1 \
    --shell-file /usr/lib/emscripten/src/shell_minimal.html \
    --preload-file ./files/ \
    -std=c++14 \
    -o main.html ./main.cpp \
&& (chromium http://localhost:8080/main.html)
