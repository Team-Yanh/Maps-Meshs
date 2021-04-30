CC= gcc
CFLAGS = -fsanitize=address -Wall -Wextra -g 
GTKFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3 `pkg-config --libs gtk+-3.0`
OGLFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
LDFLAGS = -static-libasan -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLU -lm

LDLIBS = $(shell pkg-config --libs SDL_image) -lm
FLAGS = $(CFLAGS) $(GTKFLAGS) $(LDLIBS) $(LDFLAGS)
OBJ = vector.o queue.o map.o indices.o glad.o opengl.o CompleteLine.o uiColorPick.o imageFilter.o imageUtils.o imageColoring.o display.o uiTreatment.o uiDraw.o


all: main

main: $(OBJ) main.o
	$(CC) -o main $(OBJ) main.o $(FLAGS)

main.o: main.c uiColorPick.h
	$(CC) -o main.o -c main.c $(FLAGS)

uiTreatment.o: uiTreatment.c uiTreatment.h
	$(CC) -o uiTreatment.o -c uiTreatment.c $(FLAGS)

uiColorPick.o: uiColorPick.c uiColorPick.h
	$(CC) -o uiColorPick.o -c uiColorPick.c $(FLAGS)

uiDraw.o: uiDraw.c uiDraw.h
	$(CC) -o uiDraw.o -c uiDraw.c $(FLAGS)

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
