CC= gcc
CFLAGS = -fsanitize=address -Wall -Wextra -g
LDFLAGS = -fsanitize=address -static-libasan
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

OBJ = queue.o imageFilter.o

all: main

main :${OBJ} main.o

test:${OBJ} test.o

main.o: queue.o imageFilter.o main.c

test.o: queue.o imageFilter.o test.c

imageFilter.o : queue.o imageFilter.h imageFilter.c
queue.o : queue.h queue.c

.PHONY : clean

clean:
	rm -f main *.o
