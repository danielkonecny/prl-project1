#!/bin/bash

$numbers = 16
$processes = 5

mpic++ -o pms pms.cpp pms.h

dd if=/dev/random bs=1 count=16 of=$numbers 1>/dev/null 2>/dev/null

mpirun --use-hwthread-cpus --oversubscribe -np $processes pms

rm -f pms numbers
