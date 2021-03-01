#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <limits.h>

#include "grid.h"
#include <SDL/SDL.h>
#include "display.h"

int main(int argc, char **argv)
{
    int width = 5;
    int height = 5;
    if(argc == 3)
    {
        width = strtol(argv[1], NULL, 10);
        height = strtol(argv[2], NULL, 10);
    }
    grid *g = newGrid(width, height);
    printGrid(g);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(1024, 512, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);
    drawGrid(g, screen);
    SDL_Flip(screen);

    SDL_Delay(5000);
    freeGrid(g);
    SDL_FreeSurface(screen);
    SDL_Quit();
    return 0;
}
