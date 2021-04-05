CC= gcc
CFLAGS = -fsanitize=address -Wall -Wextra -g
LDFLAGS = -fsanitize=address -static-libasan
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

OBJ = vector.o queue.o CompleteLine.o imageFilter.o imageUtils.o imageColoring.o


all: main

main :${OBJ} main.o

test:${OBJ} test.o

main.o: queue.o imageFilter.o CompleteLine.o main.c

test.o: queue.o imageFilter.o test.c

queue.o : queue.h queue.c
vector.o: vector.c vector.h 

CompleteLine.o: vector.o CompleteLine.c CompleteLine.h
imageFilter.o : imageFilter.h imageFilter.c
imageUtils.o:  queue.o imageFilter.o imageUtils.h imageUtils.c 
imageColoring.o:  queue.o imageFilter.o imageUtils.o imageColoring.h imageColoring.c

.PHONY : clean

clean:
	rm -f main test *.o
