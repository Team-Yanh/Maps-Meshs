CC= gcc -fsanitize=address -g
CFLAGS = -Wall -Wextra
OGLFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
GTKFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3 `pkg-config --libs gtk+-3.0`
LDFLAGS =
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

all: main

main: colorPicker.o main.o
	$(CC) -o main colorPicker.o main.o $(CFLAGS) $(GTKFLAGS) $(LDLIBS)

main.o: main.c colorPicker.h
	$(CC) -o main.o -c main.c $(CFLAGS) $(GTKFLAGS) $(LDLIBS)

colorPicker.o: colorPicker.c colorPicker.h
	$(CC) -o colorPicker.o -c colorPicker.c $(CFLAGS) $(GTKFLAGS) $(LDLIBS)

imageFilter.o : filters.o imageFilter.h imageFilter.c
filters.o : filters.h filters.c
queue.o : queue.h queue.c

.PHONY : clean

clean:
	rm -f opengl main *.o
