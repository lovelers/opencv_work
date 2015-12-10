opencv=pkg-config opencv --libs --cflags opencv
CC=g++

all: aeCurve.out

aeCurve.out : aeCurve.o
	$(CC) -o aeCurve.out aeCurve.o `$(opencv)`

aeCurve.o : aeCurve.cpp
	$(CC) -c aeCurve.cpp `$(opencv)`

.PHONY : clean

clean:
	rm *.o -rf;
	rm *.out -rf;
