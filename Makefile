#Makefile

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLU -lm
OBJFILES = map.o glad.o indices.o opengl.o main.o
TARGET = opengl

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

#END
