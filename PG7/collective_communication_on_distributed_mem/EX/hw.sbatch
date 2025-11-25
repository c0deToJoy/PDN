#!/bin/bash
#SBATCH --partition=debug_5min
#SBATCH --exclusive
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=4
#SBATCH --output=./jobname_%J_stdout.txt
#SBATCH --error=./jobname_%J_stderr.txt
#SBATCH --time=5:00
#SBATCH --job-name=cs4473_cs5473_hw3
#SBATCH --mail-user=youremailaddress@yourinstitution.edu
#SBATCH --mail-type=ALL
#SBATCH --chdir=./

module load OpenMPI
mpirun ./hw3.mpi
