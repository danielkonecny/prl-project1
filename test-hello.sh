#!/bin/bash

mpicc -o hello hello.c

mpirun --use-hwthread-cpus --oversubscribe -np $1 hello

rm -f hello
