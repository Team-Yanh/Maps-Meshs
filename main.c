#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

#include <gtk/gtk.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <SDL/SDL.h>

#include "SDL/SDL_image.h"
#include "imageFilter.h"
#include "imageUtils.h"
#include "imageColoring.h"
#include "colorPicker.h"
#include "CompleteLine.h"
#include "indices.h"
#include "map.h"
#include "opengl.h"
#include "display.h"

int main(int argc, char* argv[])
{
    if(argc != 3)
        err(EXIT_FAILURE, "Wrong number of argument: Requires only 2 (1 for the number of columns and one for the number of line)");
    int col = atoi(argv[1]);
    int line = atoi(argv[2]);

    opengl_Create_Terrain(col, line);

    SDL_Init(SDL_INIT_VIDEO);

    display_images();
    display_images_hugo();

    SDL_Quit();

    colorPicker();

    return 0;
}
