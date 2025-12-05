#!/usr/bin/env bash
#
# This file runs the verifier code based on the configured vars.
# You can modify this to sweep over rank sizes or other parameters.
#
# - richard.m.veras@ou.edu

# Turn on command echo for debugging 
set -x

source op_dispatch_vars.sh


DIRECTORY=$1
MIN=$2
MAX=$3
STEP=$4
NUMRANKS=$5

TIME=$(date +%F_%H%M_%S)
HOST=$(hostname -f)


# run the variants
for OP in $(ls -r $DIRECTORY/*run_verifier.x);
do
    RESULT_FILE=${OP}.${NUMRANKS}.${HOST}.${TIME}.verifier.csv
    mpiexec -n ${NUMRANKS} ${OP} ${MIN} ${MAX} ${STEP} 1 1 1 1 -1 1 -1 1 -1 ${RESULT_FILE}
    cat ${RESULT_FILE}
done

