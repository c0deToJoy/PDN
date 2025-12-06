#
#
# - richard.m.veras@ou.edu
#

CC=gcc
CFLAGS= -std=c99 -g -O2 -fopenmp

run-valgrind-local: build-local
	valgrind --leak-check=full --show-leak-kinds=all ./run-hw.x

#run-local: build-local
#	./run-hw.x


build-local: hw_code.c sparse.c
	${CC} ${CFLAGS} -c  helper.c
	${CC} ${CFLAGS} -c  hw_code.c
	${CC} ${CFLAGS} -c  sparse.c
	${CC} ${CFLAGS} hw_code.o sparse.o helper.o -o ./run-hw.x

clean:
	rm -f *~ *.x *.o
