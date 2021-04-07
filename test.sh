#!/bin/bash

PROJECT="pms"
NUMBER_FILE="numbers"
NUMBER_COUNT=16
PROCESS_COUNT=5

mpic++ -o $PROJECT $PROJECT.cpp $PROJECT.h

dd if=/dev/random bs=1 count=$NUMBER_COUNT of=$NUMBER_FILE 1>/dev/null 2>/dev/null

mpirun --use-hwthread-cpus --oversubscribe -np $PROCESS_COUNT $PROJECT

rm -f $PROJECT $NUMBER_FILE
