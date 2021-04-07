#!/bin/bash

#pocet cisel bud zadam nebo 10 :)
if [ $# -lt 1 ];then 
    numbers=10;
else
    numbers=$1;
fi;

#preklad cpp zdrojaku
mpic++ -o oets odd-even.cpp

#vyrobeni souboru s random cisly
dd if=/dev/random bs=1 count=$numbers of=numbers

#spusteni
mpirun --use-hwthread-cpus --oversubscribe -np $numbers oets

#uklid
rm -f oets numbers
