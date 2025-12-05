#!/usr/bin/env bash

########################################
# DO NOT CHANGE THIS FOLLOWING LINE:   #
# UNLESS YOU HAVE A CORRECT AND FASTER #
# IMPLEMENTATION.                      #            
OP_BASELINE_FILE="baseline_op.c"       #
########################################

######################################################
# You can even change the compiler flags if you want #
######################################################
CC=mpicc
#CFLAGS="-std=c99 -O2 -g -fopenmp"
CFLAGS="-std=c99 -O3 -mavx2 -mfma -fopenmp"

