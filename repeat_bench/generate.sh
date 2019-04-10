#!/bin/bash

rm -Rf ./temp/**
mkdir -p ./temp

for ((i=0; i<=1000; i++)); do
    FILE="./temp/s$i.cpp"
    touch $FILE
    echo -e '#include "../repeat.hpp"\n' > $FILE
    for ((j=0; j<=20; j++)); do
        echo -e "DEFINE_ME_A_FUNC\n" >> $FILE
    done
done
