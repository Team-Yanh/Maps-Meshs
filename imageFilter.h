#pragma once

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <math.h>
#include "vector.h"

typedef struct RGB
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
}RGB;

typedef struct HSV
{
    float h;
    float s;
    float v;
}HSV;

typedef struct Color
{
    SDL_PixelFormat *format;
    Uint32 pixel;
    RGB *rgb;
    HSV *hsv;

}Color;

HSV *RGBToHSV(RGB *rgb);
RGB *HSVToRGB(HSV *hsv);

Color *initColor(SDL_PixelFormat *format);
void setRGB(Color *c, Uint8 r, Uint8 g, Uint8 b);
void setHSV(Color *c, float h, float s, float v);
void setPixel(Color *c, Uint32 pixel);
void freeColor(Color *c);

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

int isValidCell(SDL_Surface *surface, int x, int y);
int isSameColor(Color *c1, Color *c2);
int isValidCellAndColor(SDL_Surface *image, int x, int y, Color *c);
int isValidNeighbour(SDL_Surface *image, int x, int y);
int isWhiteInImage(SDL_Surface *image);
