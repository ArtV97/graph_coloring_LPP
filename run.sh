#!/bin/bash

INPUTS=( "1.txt" "2.txt" "3.txt" "4.txt" "5.txt" "6.txt" "7.txt" )
seq_file="seq_result.txt"


# SEQUENTIAL EXECUTION
for input in ${INPUTS[@]}
do
    for c in $(seq 5)
    do
        echo "$input - $c" >> ${seq_file}
        { time ./seq/bin/seq_coloring data_example/${input}; } 2>&1 | grep "real" >> ${seq_file}
    done
    echo "===" >> ${seq_file}
done
