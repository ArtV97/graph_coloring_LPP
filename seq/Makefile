

all: util.o main.c
	-mkdir bin
	gcc util.o main.c -o bin/jones_plassmann

util.o: util.c
	gcc -c util.c

clean:
	rm -r bin
	rm *o