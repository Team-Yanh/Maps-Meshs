CC= gcc
CFLAGS = -fsanitize=address -Wall -Wextra -g
LDFLAGS = -fsanitize=address 
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

OBJ = queue.o imageFilter.o

all: main

main :${OBJ} main.o

test:${OBJ} test.o

main.o: queue.o imageFilter.o main.c CompleteLine.o

test.o: queue.o imageFilter.o test.c

CompleteLine.o: queue.o vector.o CompleteLine.c CompleteLine.h

imageFilter.o : queue.o imageFilter.h imageFilter.c
queue.o : queue.h queue.c

.PHONY : clean

clean:
	rm -f main *.o
