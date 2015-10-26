opencv=pkg-config opencv --libs --cflags opencv
CC=g++


all: calcHist_Demo.out histogram.out

calcHist_Demo.out : calcHist_Demo.o
	$(CC) -o calcHist_Demo.out calcHist_Demo.o `$(opencv)`

calcHist_Demo.o : calcHist_Demo.cpp
	$(CC) -c calcHist_Demo.cpp `$(opencv)`

histogram.out : histogram.o
	$(CC) -o histogram.out histogram.o `$(opencv)`

histogram.o : histogram.cpp
	$(CC) -c histogram.cpp `$(opencv)`

.PHONY : clean

clean:
	rm *.o -rf;
	rm *.out -rf;
