opencv=pkg-config opencv --libs --cflags opencv
CC=g++

all: resizing.out

resizing.out : resizing.o
	$(CC) -o resizing.out resizing.o `$(opencv)`

resizing.o : resizing.cpp
	$(CC) -c resizing.cpp `$(opencv)`

.PYTHON : clean

clean:
	@rm *.o -rf
	@rm *.out -rf
	@rm *.jpg
	@echo clean done
