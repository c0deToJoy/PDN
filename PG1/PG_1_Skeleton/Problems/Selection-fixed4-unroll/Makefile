# Note: the large spaces are tabs, this is important
# Run GCOV, requires that we build the code
# All of these runs of ./example are a test case
# We want to cover 100% of example through these tests
run-tests-coverage: build
	./example
	./example 3
	./example 8
	./example 5 4
	gcov example.c

# Run Valgrind to find memory leaks, requires that we build the code
run-tests-leak: build
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./example

# Compile the code, requires that we clean up some files first.
# Add –lm if your code needs the math library.
build: clean
	gcc -fprofile-arcs -ftest-coverage -g example.c -o example

clean:
	rm -f *~
	rm -f example
	rm -f *.gcda *.gcno *.gcov
