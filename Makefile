CC= gcc -fsanitize=address
CFLAGS = -Wall -Wextra -g
LDFLAGS =
LDLIBS = $(shell pkg-config --libs SDL_image)

OBJ = imageFilter.o main.o

all: main

main :${OBJ}

main.o: imageFilter.o main.c
imageFilter.o : imageFilter.h imageFilter.c

.PHONY : clean

clean:
	rm -f main *.o
