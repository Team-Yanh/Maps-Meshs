#include <stdio.h>
#include <gtk/gtk.h>
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

    SDL_Init(SDL_INIT_VIDEO);
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;

    image = IMG_Load("images/test5.png");

    if(image == NULL)
        errx(1, "Couldnt load image");

    SDL_Surface* screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_Rect rcDest = {0, 0, 0, 0};

    Color *black = initColor(image->format);
    Color *white = initColor(image->format);
    Color *red = initColor(image->format);
    Color *green = initColor(image->format);
    Color *orange = initColor(image->format);
    setRGB(black, 0, 0, 0);
    setRGB(white, 255, 255, 255);
    setRGB(red, 255, 0, 0);
    setRGB(green, 0, 255, 0);
    setRGB(orange, 255, 165, 0);

    //forEachPixel(image, cleanGray);

    //forEachPixel(image, keepTopoLine);
    //forEachPixel(image, keepTopoLineHSV);
    //Uint32 topoColor = SDL_MapRGB(image->format, 203, 191, 171);
    //Uint32 red = SDL_MapRGB(image->format, 255, 0, 0);

    thickenColor(image, black);

    //int counter = colorZoneBFS(image, red, 550, 300);
    //printf("counter from ColorZoneCount = %d\n", counter);
    //colorCircles(image);
    //replaceColor(image, green, black);

    //colorAllZonesFromCircles(image);

    //setMonochromatic(image, red);
    //thickenColor(image, red);
    printf("starting         \n");
    FindAllExtremity(image);

    SDL_BlitSurface(image, NULL, screen, &rcDest);
    SDL_Flip(screen);
    SDL_Delay(50);
    SDL_SaveBMP(image, "images/out.bmp");

    freeColor(black);
    freeColor(white);
    freeColor(red);
    freeColor(green);
    freeColor(orange);

    SDL_FreeSurface(image);
    SDL_FreeSurface(screen);
    SDL_Quit();

    colorPicker();

    return 0;
}
