# This Makefile orchestrates the building, verification and timing of
# your implementations.
#
#
# Run modes:
# 1. execute on machine with mpi already installed and loaded.
#    make all-local
#
# 2. If you are on a system with mpi loaded through module, then
#    you can run locally on that node for debugging by typing:
#
#    module load OpenMPI
#    make all-local
#
#
# 3. execute remotely on Schooner.
#    make all-schooner
#

# Increments to use in the local tests
# To expedite development you can adjust these,
# but for performance plots max is the minimum
# size I want to see. (HINT: for a large number
# of ranks you need to up that number to hit
# higher performance numbers)
#MIN=64
MIN=16
#MAX=8192 # good large number
#MAX=1024 # medium size
MAX=256 # small devel size
#STEP=64
STEP=16


# This is just for local runs
NUMRANKS=2

# You should not need to adjust this
MPIVER_SCHOONER="OpenMPI"
MPIVER="mpi/mpich-x86_64"


# NOTE: https://stackoverflow.com/questions/32217413/error-while-trying-to-load-module-environment-in-makefile
# make will not find "module" because by default it uses "sh" and not "bash"
SHELL:=/bin/bash
# Additionally, we have to use ":\" for each line that we want to use the module environment


find_todo:
	grep -n -i "student_todo" Makefile *.c *.sbatch *.sbatch.stub *.sh

clean:
	rm -f *.x *~ *.o
	rm -f Obj-*/{*.o,*.x,*~}

cleanall: clean
	rm -f *.csv *.png
	rm -f Obj-*/{*.png,*.csv}
	rm -rf venv

# STUDENT_TODO: Modify which sbatch file is called for your experiment.
# STUDENT_TODO: You will modify and use "./gen_sbatch.sh parallel-prob.sbatch.stub"
#               to create sbatch files for different resources allocations.
# STUDENT_TODO: Modify parallel-prob.sbatch.stub to include different experiments.
all-schooner: build-verifier-schooner  build-bench-schooner 
	sbatch parallel-prob.sbatch

# STUDENT_TODO: Fix this for later objectives.
build-verifier-schooner:
	module load ${MPIVER_SCHOONER}; \
        ./build_test_op.sh  ./Obj-00-GOTO_BLIS_ALGO/
#	./build_test_op.sh  ./Obj-B/
# ...
#	./build_test_op.sh  ./Obj-ZZZ/


# STUDENT_TODO: Fix this for later objectives.
build-bench-schooner:
	module load ${MPIVER_SCHOONER}; \
        ./build_bench_op.sh  ./Obj-00-GOTO_BLIS_ALGO/
#	./build_bench_op.sh  ./Obj-B/
# ...
#	./build_bench_op.sh  ./Obj-ZZZ/

all-local: run-verifier-local run-bench-local

# STUDENT_TODO: Fix this for later objectives.
run-verifier-local: build-verifier-local
	./run_verifier_op.sh ./Obj-A/ ${MIN} ${MAX} ${STEP} ${NUMRANKS}
	echo "Number of FAILS: `grep "FAIL" ./Obj-A/*verifier.csv|wc -l`"

	./run_verifier_op.sh ./Obj-00-GOTO_BLIS_ALGO/ ${MIN} ${MAX} ${STEP} ${NUMRANKS}
	echo "Number of FAILS: `grep "FAIL" ./Obj-00-GOTO_BLIS_ALGO/*verifier.csv|wc -l`"


#	./run_verifier_op.sh ./Obj-B/ ${MIN} ${MAX} ${STEP} ${NUMRANKS}
#	echo "Number of FAILS: `grep "FAIL" ./Obj-B/*verifier.csv|wc -l`"
#
#	./run_verifier_op.sh ./Obj-ZZZ/ ${MIN} ${MAX} ${STEP} ${NUMRANKS}
#	echo "Number of FAILS: `grep "FAIL" ./Obj-ZZZ/*verifier.csv|wc -l`"


run-bench-local: build-bench-local
	./run_bench_op.sh ./Obj-A/ ${MIN} ${MAX} ${STEP} ${NUMRANKS}
	./run_bench_op.sh ./Obj-00-GOTO_BLIS_ALGO/ ${MIN} ${MAX} ${STEP} ${NUMRANKS}


# STUDENT_TODO: Fix this for later objectives.
build-verifier-local:
	./build_test_op.sh ./Obj-A/
	./build_test_op.sh ./Obj-00-GOTO_BLIS_ALGO/
#	./build_test_op.sh ./Obj-B/
#
#	./build_test_op.sh ./Obj-ZZZ/

build-bench-local:
	./build_bench_op.sh ./Obj-A/
	./build_bench_op.sh ./Obj-00-GOTO_BLIS_ALGO/



# Virtual environments activated and used in a makefile:
# https://stackoverflow.com/questions/24736146/how-to-use-virtualenv-in-makefile
venv: venv/touchfile
venv/touchfile: requirements.txt
	test -d venv || python3 -m venv venv
	. venv/bin/activate; pip install -Ur requirements.txt
	touch venv/touchfile

plot: venv
#	. venv/bin/activate; ls ./Obj-A/*timing.csv | xargs ./plotter_multi.py "Results of Computation" "./Obj-A/PLOT_combined.png"
	. venv/bin/activate; ls ./Obj-00-GOTO_BLIS_ALGO//*timing.csv | xargs ./plotter_multi.py "Results of Computation" "./Obj-00-GOTO_BLIS_ALGO/PLOT_combined.png"

#	. venv/bin/activate; ls ./Obj-B/*timing.csv | xargs ./plotter_multi.py "Results of Computation" "./Obj-B/PLOT_combined.png"
#
#	. venv/bin/activate; ls ./Obj-ZZZ/*timing.csv | xargs ./plotter_multi.py "Results of GEMM Computation" "./Obj-ZZZ/PLOT_combined.png" 
