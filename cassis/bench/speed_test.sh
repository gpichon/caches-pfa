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
if [ "$#" -ne "$a" ]
then 
echo "Values, functions please"
else

cd ..
make
cd bench
mkdir tmp
touch tmp/values

i=0
for value in $(cat $1)
do
i=`expr $i + 1`
done

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
for value in $(cat $1)
do
echo $value
cd ../MAQAO
./get_traces.sh ../bench/speed_test/test.c $func 4 $value > /dev/null
cd ../bench/speed_test
/usr/bin/time ./bench.sh 2>&1 | awk '/WARNING|Err/ {print $0} /user/ {print 60000*substr($3,0,1)+1000*substr($3,3,2)+substr($3,6,2)}' >> ../tmp/test_$func
cd ..
done
done

python speed_test/plot_stats.py $1 $2 $i $j

rm -f ../MAQAO/trace*
rm -f ../MAQAO/instr
rm -rf tmp
fi
