CC= gcc -fsanitize=address
CFLAGS = -Wall -Wextra -g
LDFLAGS =
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

OBJ = queue.o imageFilter.o main.o test.o vector.o

all: main

main :${OBJ}

test: test.o queue.o imageFilter.o vector.o CompleteLine.o
CompleteLine.o: CompleteLine.c CompleteLine.h 
main.o: queue.o imageFilter.o main.c
imageFilter.o : queue.o imageFilter.h imageFilter.c
queue.o : queue.h queue.c

.PHONY : clean

clean:
	rm -f main *.o test
