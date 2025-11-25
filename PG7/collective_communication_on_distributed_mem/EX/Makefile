# To compile and run your code locally on a machine with MPI installed
# or a vagrant box with MPI
# (https://rantahar.github.io/introduction-to-mpi/setup.html)
# Use:
#  make compile-local 
#  make run-local
#
# To access Schooner see: https://www.ou.edu/oscer/support/machine_access
# To compile and run on schooner use
#  make compile-schooner
#  make run-schooner
#

# Note: feel free to adjust. This only affects the local run. To increase
#       the number of ranks for schooner change the sbatch file
NUMRANKS=4
#NUMRANKS=8


# You should not need to adjust this
MPIVER_SCHOONER="OpenMPI"
MPIVER="mpi/mpich-x86_64"
#MPIVER="OpenMPI"


# NOTE: https://stackoverflow.com/questions/32217413/error-while-trying-to-load-module-environment-in-makefile
# make will not find "module" because by default it uses "sh" and not "bash"
SHELL:=/bin/bash
# Additionally, we have to use ":\" for each line that we want to use the module environment



clean:
	rm -f hw.mpi *~


# Use *-local if:
# 1. You have mpi installed on your machine
#    https://rantahar.github.io/introduction-to-mpi/setup.html
# 2. You are running on a Vagrant box with mpi installed
#
# Do not use this case on schooner. 

compile-local:
	mpicc hw_code.c -o hw.mpi

run-local: compile-local
	mpiexec -n ${NUMRANKS} ./hw.mpi

# FOR SCHOONER
# Use *-schooner if you are on schooner
compile-schooner:
	module load ${MPIVER_SCHOONER}; \
	mpicc hw_code.c -o hw.mpi

run-schooner: compile-schooner
	sbatch hw.sbatch


# Don't bother with this
# *-local-module is a weird case for Richard.
compile-local-module: hw_code.c
	module load ${MPIVER}; \
	mpicc hw_code.c -o hw.mpi
run-local-module: compile-local-module
	module load ${MPIVER}; \
	mpiexec -n ${NUMRANKS} ./hw.mpi;



