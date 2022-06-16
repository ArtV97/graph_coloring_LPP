

all: util.o main.c
	-mkdir bin
	gcc util.o main.c -o bin/jones_plassmann -fopenmp

util.o: util.c
	gcc -c util.c

clean:
	rm -r bin
	rm *o