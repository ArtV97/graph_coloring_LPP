

all: util.o main.c
	-mkdir bin
	gcc util.o main.c -o bin/seq_coloring

util.o: util.c
	gcc -c util.c

clean:
	rm -r bin
	rm *o