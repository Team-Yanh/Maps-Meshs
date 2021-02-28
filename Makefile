CC= gcc -fsanitize=address
CFLAGS = -Wall -Wextra -g

OBJ = grid.o main.o

all: main
	
main :${OBJ}

main.o: main.c

grid.o: grid.c grid.h

.PHONY : clean

clean:
	rm -f main *.o
