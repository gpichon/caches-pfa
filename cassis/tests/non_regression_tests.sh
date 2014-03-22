#!/bin/bash

mkdir build
cd build
cmake ../../ -DTEST=ON
make

cd ../non_regression
./unitary_tests.sh
echo "Running some benchs"
./bench.sh > out 2>&1
echo "Diff for non regression"
diff out non_regression.txt > res_nr.txt
echo "Diff in res.txt should print nothing"
rm out
cd ..
rm -rf build
