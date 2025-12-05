#!/bin/bash
#SBATCH --partition=normal
#SBATCH --nodes=1                   # <--- Nodes for MPI
#SBATCH --ntasks=1                  # <--- Adjust these if you are mixing MPI and OpenMP
#SBATCH --ntasks-per-node=1         # <---
#SBATCH --output=./jobname_%J_stdout.txt
#SBATCH --error=./jobname_%J_stderr.txt
#SBATCH --time=5:00
#SBATCH --job-name=pdn_takehome
#SBATCH --mail-user=youremailaddress@yourinstitution.edu
#SBATCH --mail-type=ALL
#SBATCH --chdir=./


# NOTE: At first, you will need to adjust your sizes and number of implementations
#       being tested to fit in the time limit for your job, or increase your time limit. 
#
#       However, all of your schooner results should be for sizes up to 8192

# Good Resources for SLURM
# https://iamsorush.com/posts/hpc-slurm-openmp-mpi/
# Increments to use in tests
MIN=32
#MAX=8192 # good large number. This is the dimension you should aim for in your experiments
#MAX=4096 # medium size
MAX=1024 # small devel size
STEP=32


module load OpenMPI

# Let's get the stats of the nodes we are on
hostname -f # hostname of the machine
who         # who else is on this system
cat /proc/cpuinfo # low level cpu details


# Verify
./run_verifier_op.sh ./Obj-00-GOTO_BLIS_ALGO/ ${MIN} ${MAX} ${STEP} ${SLURM_NTASKS}
echo "Number of FAILS: `grep "FAIL" ./Obj-00-GOTO_BLIS_ALGO/*verifier.csv|wc -l`"

# STUDENT_TODO: Fix this for later objectives
#./run_verifier_op.sh ./Obj-B/ ${MIN} ${MAX} ${STEP} ${SLURM_NTASKS}
#echo "Number of FAILS: `grep "FAIL" ./Obj-B/*verifier.csv|wc -l`"
#
# ...
#
#./run_verifier_op.sh ./Obj-ZZZ/ ${MIN} ${MAX} ${STEP} ${SLURM_NTASKS}
#echo "Number of FAILS: `grep "FAIL" ./Obj-ZZZ/*verifier.csv|wc -l`"





# Bench the results
./run_bench_op.sh ./Obj-00-GOTO_BLIS_ALGO/ ${MIN} ${MAX} ${STEP} ${SLURM_NTASKS}

# STUDENT_TODO: Fix this for later objectives
#./run_bench_op.sh ./Obj-B/ ${MIN} ${MAX} ${STEP} ${SLURM_NTASKS}
#...
#./run_bench_op.sh ./Obj-ZZZ/ ${MIN} ${MAX} ${STEP} ${SLURM_NTASKS}


# plot the results
module unload OpenMPI
module load scikit-learn/0.23.1-foss-2020a-Python-3.8.2
module load matplotlib/3.2.1-foss-2019b-Python-3.8.2

ls ./Obj-00-GOTO_BLIS_ALGO/*timing.csv | xargs ./plotter_multi.py "Results of Computation" "./Obj-A/PLOT_schooner_combined.png"

# STUDENT_TODO: Fix this for later objectives
#ls ./Obj-B/*timing.csv | xargs ./plotter_multi.py "Results of Computation" "./Obj-B/PLOT_schooner_combined.png"
#...
#ls ./Obj-ZZZ/*timing.csv | xargs ./plotter_multi.py "Results of Computation" "./Obj-ZZZ/PLOT_schooner_combined.png" 


