CC= gcc
CFLAGS = -fsanitize=address -Wall -Wextra -g
LDFLAGS = -fsanitize=address 
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

OBJ = vector.o CompleteLine.o queue.o imageFilter.o 

all: main

main :${OBJ} main.o

test:${OBJ} test.o

main.o: queue.o imageFilter.o CompleteLine.o main.c

test.o: queue.o imageFilter.o test.c


CompleteLine.o: queue.o vector.o CompleteLine.c CompleteLine.h
imageFilter.o : queue.o imageFilter.h imageFilter.c
queue.o : queue.h queue.c
vector.o: vector.c vector.h 

.PHONY : clean

clean:
	rm -f main *.o
