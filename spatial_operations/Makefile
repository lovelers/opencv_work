opencv=pkg-config opencv --libs --cflags opencv
CC=g++

all: single-pixel-operations.out neighborhood-operations.out geometric-spatial-transformations.out gamma-correction.out

single-pixel-operations.out : single-pixel-operations.o
	$(CC) -o single-pixel-operations.out single-pixel-operations.o `$(opencv)`

single-pixel-operations.o : single-pixel-operations.cpp
	$(CC) -c single-pixel-operations.cpp `$(opencv)`

neighborhood-operations.out : neighborhood-operations.o
	$(CC) -o neighborhood-operations.out neighborhood-operations.o `$(opencv)`

neighborhood-operations.o : neighborhood-operations.cpp
	$(CC) -c neighborhood-operations.cpp `$(opencv)`

geometric-spatial-transformations.out : geometric-spatial-transformations.o
	$(CC) -o geometric-spatial-transformations.out geometric-spatial-transformations.o `$(opencv)`

geometric-spatial-transformations.o : geometric-spatial-transformations.cpp
	$(CC) -c geometric-spatial-transformations.cpp `$(opencv)`

gamma-correction.out : gamma-correction.o
	$(CC) -o gamma-correction.out gamma-correction.o `$(opencv)`

gamma-correction.o : gamma-correction.cpp
	$(CC) -c gamma-correction.cpp `$(opencv)`

.PYTON: clean

clean:
	rm *.o -rf
	rm *.out -rf
