# run with 'make -i'
# Note: the large spaces are tabs, this is important
# Run GCOV, requires that we build the code
# All of these runs of ./prog are a test case
# We want to cover 100% of prog through these tests
run-tests-coverage: build
	./prog
	./prog 3 3 1 2 3 4 5 6 7 8 9 #Test 3x3 matrix
	./prog 1 4 10 20 30 40 #Test 1x4 matrix
	./prog 4 1 5 6 7 8 #Test 4x1 matrix
	./prog 2 5 1 2 3 4 5 6 7 8 9 10 #Test 2x5 matrix
	./prog 5 2 1 2 3 4 5 6 7 8 9 10 #Test 5x2 matrix
	./prog 2 4 0 -1 2 -3 4 0 #Test 2x4 matrix with negative numbers
	echo "STUDENT TODO: What are all the parameter combinations that cover your code 100%?"
	gcov prog.c
# Run Valgrind to find memory leaks, requires that we build the code
run-tests-leak: build
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 3 3 1 2 3 4 5 6 7 8 9
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 1 4 10 20 30 40
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 4 1 5 6 7 8
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 2 5 1 2 3 4 5 6 7 8 9 10
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 5 2 1 2 3 4 5 6 7 8 9 10
	echo "STUDENT TODO: What are all the parameter combinations that cover your code 100%?"
	echo "              These tests should match your coverage tests."

# Compile the code, requires that we clean up some files first.
# Add –lm if your code needs the math library.
build: clean
	gcc -fprofile-arcs -ftest-coverage -g prog.c -o prog
clean:
	rm -f *~
	rm -f prog
	rm -f *.gcda *.gcno *.gcov
