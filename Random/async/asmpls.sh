#!/bin/bash

clang++ -std=c++1z -I/home/vittorioromeo/OHWorkspace/vrm_core/include -DNDEBUG -Ofast -S -mllvm --x86-asm-syntax=intel  \
        -I/home/vittorioromeo/OHWorkspace/vrm_pp/include \
        -I/home/vittorioromeo/OHWorkspace/ecst/include \
        -fno-exceptions -fno-rtti $1 -o asm.s
