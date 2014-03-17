#!/bin/bash
make
cd non_regression
./unitary_tests.sh
echo "Running some benchs"
./bench.sh > out 2>&1
echo "Diff for non regression"
diff out non_regression.txt
echo "Diff should print nothing"
#rm out
cd ..
