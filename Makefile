CC= gcc -fsanitize=address
CFLAGS = -Wall -Wextra -g
LDFLAGS = 
LDLIBS = $(shell pkg-config --libs sdl)

OBJ = grid.o display.o main.o

all: main
	
main :${OBJ}

main.o: main.c

grid.o: grid.c grid.h

display.o: display.c display.h

.PHONY : clean

clean:
	rm -f main *.o
