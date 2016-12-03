#!/bin/bash

FLAGS="-std=c++1z -Ofast"
GCC="g++"
TIME='/usr/bin/time -f %e'

$GCC $FLAGS $1 -o /tmp/x0.x
$GCC $FLAGS $2 -o /tmp/x1.x
GCC_EXEC_TIME_0=$($TIME /tmp/x0.x)
GCC_EXEC_TIME_1=$($TIME /tmp/x1.x)
echo $GCC_EXEC_TIME_0
echo $GCC_EXEC_TIME_1


$GCC $FLAGS $1 -S -o /tmp/x0.s
$GCC $FLAGS $2 -S -o /tmp/x1.s
GCC_ASM_BYTES_0=$(wc -c /tmp/x0.s | cut -f1 -d' ')
GCC_ASM_BYTES_1=$(wc -c /tmp/x1.s | cut -f1 -d' ')
echo $GCC_ASM_BYTES_0
echo $GCC_ASM_BYTES_1
