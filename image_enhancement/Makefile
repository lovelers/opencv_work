opencv=pkg-config opencv --libs --cflags opencv
CC=g++

all: image-enhancement.out

image-enhancement.out : image-enhancement.o
	$(CC) -o image-enhancement.out image-enhancement.o `$(opencv)`

image-enhancement.o : image-enhancement.cpp
	$(CC) -c image-enhancement.cpp `$(opencv)`

.PYTHON : clean

clean:
	@rm *.o -rf
	@rm *.out -rf
	@rm *.jpg
	@echo clean done
