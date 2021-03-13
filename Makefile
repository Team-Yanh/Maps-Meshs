CC= gcc -fsanitize=address
CFLAGS = -Wall -Wextra -g
LDFLAGS =
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

OBJ = queue.o imageFilter.o main.o

all: main

main :${OBJ}

main.o: queue.o imageFilter.o main.c
imageFilter.o : queue.o imageFilter.h imageFilter.c
queue.o : queue.h queue.c

.PHONY : clean

clean:
	rm -f main *.o
