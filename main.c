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
#include "CompleteLine.h"
#include "indices.h"
#include "map.h"
#include "opengl.h"
#include "display.h"
#include "uiTreatment.h"

int main()
{
    uiTreatment();

    return 0;
}
