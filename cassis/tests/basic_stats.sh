#!/bin/bash

a=2 
if [ "$#" -le "$a" ]
then 
echo "Size, functions, cache level please"
else

cd ..
make
cd tests
mkdir tmp
touch tmp/values

j=0
for value in $(cat $2)
do
j=`expr $j + 1`
done

echo "Running some benchs"
echo "Time for the test  with x values in y ms."
for func in $(cat $2)
do
touch tmp/test_$func
echo "---------------------------------"
echo "Running "$func" test "
cd ../MAQAO
./get_traces.sh ../tests/speed_test/test.c $func 4 $1 > /dev/null
cd ../tests/speed_test
./../../bin/cassis -f ../architectures/architest.cassis_1.xml -t 4 -v 1 -r ../../MAQAO -b no_stack -o| awk '/'"$3"'/ {print $0}' | awk '/misses/ {print substr($4, 0, length($4)-1)} /hits/ {print substr($6, 0, length($6)-1)} /writes_back/ {print substr($8, 0, length($8)-1)}' >> ../tmp/test_$func
cd ..
done

python basic_stats/plot_stats.py $2 $j

rm -f ../MAQAO/trace*
rm -f ../MAQAO/instr
rm -rf tmp
fi
