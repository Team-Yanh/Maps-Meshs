#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include "indices.h"
#include "map.h"
#include <err.h>
#include <stdlib.h>
#include "opengl.h"
#include <math.h>

int main(int argc, char* argv[])
{
    if(argc != 3)
        err(EXIT_FAILURE, "Wrong number of argument: Requires only 2 (1 for the number of columns and one for the number of line)");
    int col = atoi(argv[1]);
    int line = atoi(argv[2]);

    opengl_Create_Terrain(col, line);
    return 0;
}
