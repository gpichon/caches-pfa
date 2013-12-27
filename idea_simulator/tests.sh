#!/bin/bash

make clean
make
make testreplacement
make testarchi
make testmesi

valgrind ./bin/testreplacement
valgrind ./bin/testarchi
valgrind ./bin/testmesi