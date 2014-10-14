#!/bin/bash

cd SSVCMake
git commit -a -m "progress"
git push

cd ..
git ws do "(cd extlibs && cd SSVCMake && git reset --hard HEAD && git pull origin master)"
sudo git ws do "rm ./build/CMakeCache.txt"
sudo git ws do "rm ./lib/CMakeCache.txt"