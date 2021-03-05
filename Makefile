CC= gcc -fsanitize=address
CFLAGS = -Wall -Wextra -g
LDFLAGS =
LDLIBS = $(shell pkg-config --libs SDL_image)

OBJ = main.o

all: main

main :${OBJ}

main.o: main.c


.PHONY : clean

clean:
	rm -f main *.o
