#!/bin/bash

make clean
make test

valgrind ./bin/test