#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"

#include "imageFilter.h"
#include "queue.h"

int main(void /*int argc, char **argv*/)
{
    SDL_Init(SDL_INIT_VIDEO);
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;
    image = IMG_Load("images/test1.png");
    if(image == NULL)
        errx(1, "Couldnt load image");

    SDL_Surface* screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);


    SDL_Rect rcDest = {0, 0, 0, 0};

    forEachPixel(image, cleanGray);
    forEachPixel(image, keepTopoLine);
    //forEachPixel(image, keepTopoLineHSV);
    Uint32 topoColor = SDL_MapRGB(image->format, 203, 191, 171);
    Uint32 red = SDL_MapRGB(image->format, 255, 0, 0);
    //setMonochromatic(image, red);
    //thickenColor(image, red);

    SDL_BlitSurface(image, NULL, screen, &rcDest);
    SDL_Flip(screen);

    SDL_Delay(5000);
    SDL_SaveBMP(image, "images/out.bmp");
    SDL_FreeSurface(image);
    SDL_FreeSurface(screen);
    SDL_Quit();

    return 0;
}
