#!/bin/bash

cd ../build
valgrind ./bin/test
cd ../non_regression
