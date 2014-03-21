#!/bin/bash

a=3
b=4

if [ "$a" -le "$#" ]
then

export OMP_NUM_THREADS=$3

echo "Program file"
echo $1

echo "Function to trace"
echo $2

echo "Number of threads"
echo $3

gcc -o test $1 -DKERNEL=$2 -fopenmp -g
./maqao instrumentation.lua test "$2.*" > instr

if [ "$b" == "$#" ]
then
echo "Program parameter"
echo $4
./test_i_mtl $4 2> out
else
./test_i_mtl 2> out
fi

awk '/STORE|LOAD/ {print $0 > "trace"$3}' out

echo "Removing files"
rm -f test_i_mtl
rm -f test
rm -f trace.MTL.*
rm -f madras*
rm -f out

else echo "Usage: ./get_traces file.c name_function number_threads"
fi
