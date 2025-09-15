# run with 'make -i'
# Note: the large spaces are tabs, this is important
# Run GCOV, requires that we build the code
# All of these runs of ./prog are a test case
# We want to cover 100% of prog through these tests
run-tests-coverage: build
	./prog
	./prog 3
	./prog 8
	./prog 5 4
	gcov prog.c
# Run Valgrind to find memory leaks, requires that we build the code
run-tests-leak: build
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 3
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 8
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog 5 4

# Compile the code, requires that we clean up some files first.
# Add –lm if your code needs the math library.
build: clean
	gcc -fprofile-arcs -ftest-coverage -g prog.c -o prog
clean:
	rm -f *~
	rm -f prog
	rm -f *.gcda *.gcno *.gcov
