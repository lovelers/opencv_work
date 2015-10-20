opencv=pkg-config opencv --libs --cflags opencv
CC=g++


all: dft.out

dft.out : dft.o
	$(CC) -o dft.out dft.o `$(opencv)`

dft.o : dft.cpp
	$(CC) -c dft.cpp `$(opencv)`
