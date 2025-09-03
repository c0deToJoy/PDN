# run with 'make -i'
# Note: the large spaces are tabs, this is important
# Run GCOV, requires that we build the code
# All of these runs of ./prog are a test case
# We want to cover 100% of prog through these tests
run-tests-coverage: build
	./prog
	./prog 1 2 3 4 5 6 7 #Test with a sorted array
	./prog 7 6 5 4 3 2 1 #Test with a reverse sorted array
	./prog 7 7 7 7 7 7 7 #Test with all identical elements
	./prog 7 #Test with a single element
	./prog 0 #Test with an empty array
	./prog 1 1 7 7 6 4 4 8 8 #Test with many duplicates
	./prog -7 6 -4 -8 8 #Test negative numbers
	./prog 9 2 8 4 0 1 #Test even amount of numbers
	echo "STUDENT TODO: What are all the parameter combinations that cover your code 100%?"
	gcov prog.c
# Run Valgrind to find memory leaks, requires that we build the code
run-tests-leak: build
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 1 2 3 4 5 6 7
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 7 6 5 4 3 2 1
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 7 7 7 7 7 7 7
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 7
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 0
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 1 1 7 7 6 4 4 8 8
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog -7 6 -4 -8 8
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 9 2 8 4 0 1
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
