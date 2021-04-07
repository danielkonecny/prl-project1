#!/bin/bash

if [ $# -lt 1 ];then 
    numbers=16;
else
    numbers=$1;
fi;

mpic++ -o pms pms.cpp pms.h

dd if=/dev/random bs=1 count=$numbers of=numbers

mpirun --use-hwthread-cpus --oversubscribe -np $numbers pms

rm -f pms numbers
