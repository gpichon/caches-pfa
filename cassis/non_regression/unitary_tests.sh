#!/bin/bash
cd ..
make test
valgrind ./bin/test
cd non_regression
