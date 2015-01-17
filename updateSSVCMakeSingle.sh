#!/bin/bash

cd SSVCMake
git commit -a -m "progress"
git push

cd ..
cd "$1" &&\
(cd extlibs && cd SSVCMake && git reset --hard HEAD && git pull origin master) &&\
sudo rm "./build/CMakeCache.txt" 
sudo rm "./lib/CMakeCache.txt"