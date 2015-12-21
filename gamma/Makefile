opencv=pkg-config opencv --libs --cflags opencv
CC=g++

all: samGamma.out albertGamma.out willisGamma.out gammaRatio.out

samGamma.out : samGamma.o
	$(CC) -o samGamma.out samGamma.o `$(opencv)`

albertGamma.out : albertGamma.o
	$(CC) -o albertGamma.out albertGamma.o `$(opencv)`

willisGamma.out : willisGamma.o
	$(CC) -o willisGamma.out willisGamma.o `$(opencv)`

gammaRatio.out : gammaRatio.o
	$(CC) -o gammaRatio.out gammaRatio.o `$(opencv)`

samGamma.o : samGamma.cpp
	$(CC) -c samGamma.cpp `$(opencv)`

albertGamma.o : albertGamma.cpp
	$(CC) -c albertGamma.cpp `$(opencv)`

willisGamma.o : willisGamma.cpp
	$(CC) -c willisGamma.cpp `$(opencv)`

gammaRatio.o : gammaRatio.cpp
	$(CC) -c gammaRatio.cpp `$(opencv)`



.PHONY : clean

clean:
	rm *.o -rf;
	rm *.out -rf;
