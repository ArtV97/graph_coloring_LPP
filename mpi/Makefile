

all: graph.o util.o main.c
	-mkdir bin
	mpicc graph.o util.o main.c -o bin/jones_plassmann

graph.o: graph.c
	gcc -c graph.c

util.o: util.c
	gcc -c util.c

clean:
	rm -r bin
	rm *o