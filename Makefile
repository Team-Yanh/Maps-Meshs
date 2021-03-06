CC= gcc -fsanitize=address
CFLAGS = -Wall -Wextra -g
LDFLAGS =
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

OBJ = filters.o imageFilter.o main.o

all: main

main :${OBJ}

main.o: imageFilter.o main.c
imageFilter.o : filters.o imageFilter.h imageFilter.c
filters.o : filters.h filters.c

.PHONY : clean

clean:
	rm -f main *.o
