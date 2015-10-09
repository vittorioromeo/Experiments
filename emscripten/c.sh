#!/bin/zsh

/usr/lib/emscripten/em++ \
    --shell-file /usr/lib/emscripten/src/shell_minimal.html \
    --preload-file ./files/** \
    -std=c++14 \
    -o main.html ./main.cpp \
&& (chromium http://localhost:8080/main.html)
