

all: graph.o util.o main.c
	-mkdir bin
	gcc graph.o util.o main.c -o bin/seq_coloring

graph.o: graph.c
	gcc -c graph.c

util.o: util.c
	gcc -c util.c

clean:
	rm -r bin
	rm *o