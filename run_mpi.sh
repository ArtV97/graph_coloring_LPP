#!/bin/bash

if [ $# != 2 ]
then
    echo "missing parameters: <num_MPI_processess>"
    exit 1
fi

NUM_PROCESSES=$1

INPUTS=( "1.txt" "2.txt" "3.txt" "4.txt" "5.txt" "6.txt" "7.txt" )
mpi_file="mpi_${NUM_PROCESSES}_result.txt"


# MPI EXECUTION
for input in ${INPUTS[@]}
do
    for c in $(seq 5)
    do
        echo "$input - $c" >> ${mpi_file}
        mpiexec -n ${NUM_PROCESSES} ./mpi/bin/jones_plassmann data_example/${input} | tail -n 1 | cut -d " " -f 4  >> ${mpi_file}
    done
    echo "===" >> ${mpi_file}
done