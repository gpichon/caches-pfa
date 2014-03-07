#!/bin/bash

a=3

if [ "$#" == "$a" ]
then echo "OK"

export OMP_NUM_THREADS=$3

echo "Fichier de test"
echo $1

echo "Fonction a tester"
echo $2

echo "Nombre de threads"
echo $3

gcc -o test $1 -DKERNEL=par -fopenmp -g
./maqao instrumentation.lua test "$2.*" > /dev/null
./test_i_mtl 2> out

i=0
while ((i < $3))
do
cat out | awk '$3 ~ /'"$i"'/' > trace$i
((i += 1))
done 

else echo "PAS OK"
fi

echo "Suppressions diverses"
rm test_i_mtl
rm test
rm trace.MTL.*
rm madras*
rm out
