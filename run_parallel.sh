#!/bin/bash

if [ $# != 2 ]
then
    echo "missing parameters: <num_openMP_threads> <num_MPI_processess>"
    exit 1
fi

NUM_THREADS=$1
NUM_PROCESSES=$2

INPUTS=( "1.txt" "2.txt" "3.txt" "4.txt" "5.txt" "6.txt" "7.txt" )
openMP_file="openmp_${NUM_THREADS}_result.txt"
mpi_file="mpi_${NUM_PROCESSES}_result.txt"


# OpenMP EXECUTION
for input in ${INPUTS[@]}
do
    for c in $(seq 5)
    do
        echo "$input - $c" >> ${openMP_file}
        { time ./openMP/bin/jones_plassmann ${NUM_THREADS} data_example/${input}; } 2>&1 | grep "real" >> ${openMP_file}
    done
    echo "===" >> ${openMP_file}
done

# MPI EXECUTION
for input in ${INPUTS[@]}
do
    for c in $(seq 5)
    do
        echo "$input - $c" >> ${mpi_file}
        { time mpiexec -n ${NUM_PROCESSES} ./mpi/bin/jones_plassmann data_example/${input}; } 2>&1 | grep "real" >> ${mpi_file}
    done
    echo "===" >> ${mpi_file}
done