#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

#include <SDL/SDL.h>

#include "SDL/SDL_image.h"
#include "imageFilter.h"
#include "imageUtils.h"
#include "imageColoring.h"
#include "CompleteLine.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;

    image = IMG_Load("images/test2.png");

    if(image == NULL)
        errx(1, "Couldnt load image");

    SDL_Surface* screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_Rect rcDest = {0, 0, 0, 0};

    /*
    Point *p1 = malloc(sizeof(Point));
    Point *p2 = malloc(sizeof(Point));
    p1->x = 100;
    p1->y = 100;
    p2->x = 215;
    p2->y = 120;
    */

    FindAllExtremity(image);
    
    SDL_BlitSurface(image, NULL, screen, &rcDest);
    SDL_Flip(screen);
    SDL_Delay(5000);
    SDL_SaveBMP(image, "images/out.bmp");
    //freeColor(black);

    SDL_FreeSurface(image);
    SDL_FreeSurface(screen);
    SDL_Quit();


    return 0;
}
