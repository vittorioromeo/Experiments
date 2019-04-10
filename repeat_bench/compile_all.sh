#!/bin/bash

for ((i=0; i<=1000; i++)); do
    FILE="./temp/s$i.cpp"
    g++ $FILE
done
