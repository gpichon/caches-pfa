#!/bin/bash

make clean
make
make testreplacement

valgrind ./bin/testreplacement