CC= gcc
CFLAGS = -fsanitize=address -Wall -Wextra -g
LDFLAGS =  -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLU -lm
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

FLAGS = $(CFLAGS) $(GTKFLAGS) $(LDLIBS) $(LDFLAGS)

OBJ = vector.o queue.o CompleteLine.o imageFilter.o imageUtils.o imageColoring.o

all: main

main: $(OBJ) main.o
	$(CC) -o main $(OBJ) main.o $(FLAGS)

main.o: main.c colorPicker.h
	$(CC) -o main.o -c main.c $(FLAGS)

colorPicker.o: colorPicker.c colorPicker.h
	$(CC) -o colorPicker.o -c colorPicker.c $(FLAGS)


test: ${OBJ} test.o
test.o: queue.o imageFilter.o test.c

queue.o : queue.h queue.c
vector.o: vector.c vector.h 

CompleteLine.o: vector.o CompleteLine.c CompleteLine.h
imageFilter.o : imageFilter.h imageFilter.c
imageUtils.o:  queue.o imageFilter.o imageUtils.h imageUtils.c 
imageColoring.o:  queue.o imageFilter.o imageUtils.o imageColoring.h imageColoring.c

.PHONY : clean

clean:
	rm -f main test opengl *.o
