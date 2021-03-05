#include "imageFilter.h"

Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

void forEachPixel(SDL_Surface *image, void (*filter)(Uint8 *r, Uint8 *g, Uint8 *b))
{
    Uint8 r;
    Uint8 g;
    Uint8 b;

    SDL_LockSurface(image);
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            SDL_GetRGB(getPixel(image, i, j), image->format, &r, &g, &b);
            filter(&r, &g, &b);
            putPixel(image, i, j, SDL_MapRGB(image->format, r, g, b) );
        }
    }
    SDL_UnlockSurface(image);
}
