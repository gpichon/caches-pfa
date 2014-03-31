#/*
# * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
# * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
# *
# * This Source Code Form is subject to the terms of the Mozilla Public
# * License, v. 2.0. If a copy of the MPL was not distributed with this
# * file, You can obtain one at http://mozilla.org/MPL/2.0/.
# */

#!/bin/bash

a=2 
if [ "$#" -le "$a" ]
then 
echo "Size, functions, cache level please"
else

cd ..
make
cd bench
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
./get_traces.sh ../bench/speed_test/test.c $func 4 $1 > /dev/null
cd ../bench/speed_test
./../../bin/cassis -f ../architectures/architest_INTEL.cassis.xml -t 4 -v 1 -r ../../MAQAO -b no_stack -o| awk '/'"$3"'/ {print $0}' | awk '/misses/ {print substr($4, 0, length($4)-1)} /hits/ {print substr($6, 0, length($6)-1)} /writes_back/ {print substr($8, 0, length($8)-1)}' >> ../tmp/test_$func
cd ..
done

python basic_stats/plot_stats.py $2 $j $1 $3

rm -f ../MAQAO/trace*
rm -f ../MAQAO/instr
rm -rf tmp
fi
