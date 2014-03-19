#!/bin/bash

a=1

if [ "$#" == "$a" ]
then echo "OK"

echo "Line to follow"
echo $1

awk '$0 ~ /SRC@'"$1"' / {print "instr:"$1}' instr

else echo "\nUsage: ./follow_instr line_number"
fi
