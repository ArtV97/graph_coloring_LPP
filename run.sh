#!/bin/bash

INPUTS=( "1.txt" "2.txt" "3.txt" "4.txt" "5.txt" "6.txt" "7.txt" )
seq_file="seq_result.txt"


# SEQUENTIAL EXECUTION
for input in ${INPUTS[@]}
do
    for c in $(seq 5)
    do
        echo "$input - $c" >> ${seq_file}
        ./seq/bin/seq_coloring data_example/${input} | tail -n 1 | cut -d " " -f 4 >> ${seq_file}
    done
    echo "===" >> ${seq_file}
done
