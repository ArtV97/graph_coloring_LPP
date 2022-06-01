

all: util.o main.c
	-mkdir bin
	mpicc util.o main.c -o bin/2coloring

util.o: util.c
	gcc -c util.c

clean:
	rm -r bin
	rm *o