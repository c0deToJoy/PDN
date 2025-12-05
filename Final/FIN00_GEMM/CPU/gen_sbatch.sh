#!/usr/bin/env bash
#
# This script will generate a range of sbatch files that
# sweep over different amounts of resources.
#
# Go through the variables and set them for your experiments
#
# EMAIL=
# FILE_STUB_SBATCH=
# PARTITION=
# DURATION=
# NODE_RANGE=
# TASK_RANGE=
# CPU_RANGE=
#
#
# After generating you can run all of the sbatch scripts with the following:
#   for i in `ls sbgen_*.sbatch`; do sbatch $i || true ; done
#
#
# - richard.m.veras@ou.edu
#

# Set this so you get alerts when your job is done.
EMAIL="YYYY_TODO_CHANGE_ME_YYY@ou.edu"

# This should be a script of the code you want run in your
# sbatch files.
FILE_STUB_SBATCH=parallel.sbatch.stub


# OSCER keeps a partial list of parititons here:
#  https://www.ou.edu/oscer/support/partitions
# You can see what partitions are being used with:
#  squeue | awk '{print $2}' | sort -u
#
# General purpose parititions. Every OSCER user can access these.
PARTITION=debug
#PARTITION=debug_5min # Very fast, but duration must be "00:05:00" or less
#PARTITION=normal
#PARTITION=gpu
#PARTITION=gpu_a100
#PARTITION=debug_gpu # In concept a good idea, but these might still be very old and unsupported GPUs.

# USE THESE!!!!!!!!!
#
# CS Students have priority access to the following and should use these:
# oucs_gpu, oucs_gpu_48gb_dual: c1046
# oucspdn_cpu: 269 nodes
# oucspdn_gpu: 14 nodes
#PARTITION=oucspdn_cpu
#PARTITION=oucspdn_gpu


# If you have DISC membership you can use these parititions.
# https://www.ou.edu/disc/about/people/disc-membership
# https://www.ou.edu/disc/resources/cpu-and-gpu-resources-on-the-oscer-supercomputer
#PARITION=disc 
#PARITION=disc_largemem
#PARITION=disc_dual_a100
#PARITION=disc_quad_a100
#PARITION=disc_dual_h100

# Estimate how much time you will need. 48hrs is the max for most queues. Note
# that the smaller the duration, the faster you will queue (Shortest Job First).
DURATION="00:02:00"
#DURATION="00:05:00"
#DURATION="00:15:00"


# Adjust these ranges. Note not all combinations will work
# for every partitions as they have certain limits.

# How many physical systems will you use?
NODE_RANGE="1 2 4"
# How many MPI ranks per physical system will you use?
TASK_RANGE="1 2 4"
# How many threads per rank will you use?
CPU_RANGE="1 2 4 8"

# Generate all of the SBATCH files
#
for NUM_NODES in $NODE_RANGE;
do
    for NTASK_PER_NODE in $TASK_RANGE;
    do
	for CPUS_PER_TASK in $CPU_RANGE;
	do
	    SBATCH_FILE_NAME=sbgen_parallel_${PARTITION}_${NUM_NODES}nodes_${NTASK_PER_NODE}tasks_${CPUS_PER_TASK}cpu.sbatch
	    cat <<EOF > ${SBATCH_FILE_NAME}
#!/usr/bin/env bash
#
#SBATCH --partition=${PARTITION}
#SBATCH --time=${DURATION}
#
#SBATCH --nodes=${NUM_NODES}
#SBATCH --ntasks-per-node=${NTASK_PER_NODE}
#SBATCH --cpus-per-task=${CPUS_PER_TASK}
#
#
#SBATCH --mem=1024
#
#SBATCH --job-name=jobname
#SBATCH --output=./job/jobname_%J_stdout.txt
#SBATCH --error=./job/jobname_%J_stderr.txt
#SBATCH --mail-user=${EMAIL}
#SBATCH --mail-type=ALL
#SBATCH --chdir=./
#
#################################################
EOF

	    cat ${FILE_STUB_SBATCH} >> ${SBATCH_FILE_NAME}
	done
    done
done
