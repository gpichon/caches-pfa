#!/bin/bash

make clean
make
make testreplacement
make testarchi
make testmesi

valgrind ./bin/testreplacement
valgrind ./bin/testarchi 1> /dev/null
valgrind ./bin/testmesi