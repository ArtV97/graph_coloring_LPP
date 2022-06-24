#!/bin/bash

if [ $# != 1 ]
then
    echo "missing parameters: <num_openMP_threads>"
    exit 1
fi

NUM_THREADS=$1

INPUTS=( "1.txt" "2.txt" "3.txt" "4.txt" "5.txt" "6.txt" "7.txt" )
openMP_file="openmp_${NUM_THREADS}_result.txt"


# OpenMP EXECUTION
for input in ${INPUTS[@]}
do
    for c in $(seq 5)
    do
        echo "$input - $c" >> ${openMP_file}
        ./openMP/bin/jones_plassmann ${NUM_THREADS} data_example/${input} | tail -n 1 | cut -d " " -f 4  >> ${openMP_file}
    done
    echo "===" >> ${openMP_file}
done
