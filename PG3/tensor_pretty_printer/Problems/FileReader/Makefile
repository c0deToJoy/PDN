# Add/edit test cases as necessary
run-tests-coverage: build
	./prog input_0001.txt output_0001.txt
	./prog input_0002.txt output_0002.txt
	./prog input_0003.txt output_0003.txt
	gcov prog.c

# Add/edit test cases as necessary
run-tests-leak: build
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog input_0001.txt output_0001.txt
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog input_0002.txt output_0002.txt
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./prog input_0003.txt output_0003.txt

build: clean
	gcc -fprofile-arcs -ftest-coverage -g prog.c -o prog

clean:
	rm -f *~
	rm -f prog
	rm -f *.gc*
	rm -f *out*
