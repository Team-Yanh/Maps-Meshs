CC= gcc -fsanitize=address
CFLAGS = -Wall -Wextra -g 
OGLFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
GTKFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3 `pkg-config --libs gtk+-3.0`
LDFLAGS =
LDLIBS = $(shell pkg-config --libs SDL_image) -lm

gtk: gtk.o
	$(CC) -o gtk gtk.o $(CFLAGS) $(GTKFLAGS)

opengl: glad.o opengl.o
	$(CC) -o opengl glad.o opengl.o $(CFLAGS) $(OGLFLAGS)

gtk.o: gtk.c
	$(CC) -o gtk.o -c gtk.c $(CFLAGS) $(GTKFLAGS)

opengl.o : glad.o opengl.c
	$(CC) -o opengl.o -c opengl.c $(CFLAGS) $(OGLFLAGS)

glad.o : glad.c
	$(CC) -o glad.o -c glad.c $(CFLAGS)

imageFilter.o : filters.o imageFilter.h imageFilter.c
filters.o : filters.h filters.c
queue.o : queue.h queue.c

.PHONY : clean

clean:
	rm -f opengl gtk *.o
