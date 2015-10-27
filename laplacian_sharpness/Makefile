opencv=pkg-config opencv --libs --cflags opencv
CC=g++

all: laplacian-sharpness.out

laplacian-sharpness.out : laplacian-sharpness.o
	$(CC) -o laplacian-sharpness.out laplacian-sharpness.o `$(opencv)`

laplacian-sharpness.o : laplacian-sharpness.cpp
	$(CC) -c laplacian-sharpness.cpp `$(opencv)`

.PYTHON : clean

clean:
	@rm *.o -rf
	@rm *.out -rf
	@echo clean done
