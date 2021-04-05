CC= gcc
CFLAGS = -fsanitize=address -Wall -Wextra -g
LDFLAGS = -fsanitize=address -static-libasan
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

OBJ = queue.o imageFilter.o imageUtils.o imageColoring.o

all: main

main :${OBJ} main.o

test:${OBJ} test.o

main.o: queue.o imageFilter.o main.c

test.o: queue.o imageFilter.o test.c

queue.o : queue.h queue.c

imageFilter.o : imageFilter.h imageFilter.c
imageUtils.o:  queue.o imageFilter.o imageUtils.h imageUtils.c 
imageColoring.o:  queue.o imageFilter.o imageUtils.o imageColoring.h imageColoring.c

.PHONY : clean

clean:
	rm -f main *.o
