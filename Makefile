CC= gcc -fsanitize=address
CFLAGS = -Wall -Wextra -g -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
LDFLAGS =
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

OBJ = queue.o filters.o imageFilter.o main.o

all: main

main : glad.o main.o
	$(CC) -o main glad.o main.o $(CFLAGS)

main.o : glad.o main.c
	$(CC) -o main.o -c main.c $(CFLAGS)

glad.o : glad.c
	$(CC) -o glad.o -c glad.c $(CFLAGS)

imageFilter.o : filters.o imageFilter.h imageFilter.c
filters.o : filters.h filters.c
queue.o : queue.h queue.c

.PHONY : clean

clean:
	rm -f main *.o
