#!/bin/bash

mkdir build
cd build
cmake ../../ -DTEST=ON #-DLUA_LIBRARIES=-llua5.2 -DLUA_INCLUDE_DIR=/usr/include/lua5.2
make

cd ../non_regression
./unitary_tests.sh
echo "Running some benchs"
./bench.sh > out 2>&1
echo "Diff for non regression"
diff out non_regression.txt > res_nr.txt
echo "Diff in non_regression/res_nr.txt should print nothing"
rm out
cd ..
rm -rf build
