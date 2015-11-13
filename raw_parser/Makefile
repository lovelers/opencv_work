opencv=pkg-config opencv --libs --cflags opencv
CC=g++

all: raw-parser.out

raw-parser.out : raw-parser.o
	$(CC) -o raw-parser.out raw-parser.o `$(opencv)`

raw-parser.o : raw-parser.cpp
	$(CC) -c raw-parser.cpp `$(opencv)`

.PYTON: clean

clean:
	rm *.o -rf
	rm *.out -rf
