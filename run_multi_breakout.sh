#!/bin/bash

g++ -std=c++2a -Wall -Wextra -Wpedantic -lpthread \
    -lsfml-system -lsfml-graphics -lsfml-window \
    -fdiagnostics-color=always \
    $1 -o /tmp/x.x && /tmp/x.x
