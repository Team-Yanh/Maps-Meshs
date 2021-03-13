#pragma once

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <math.h>
#include "queue.h"

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
void forEachPixel(SDL_Surface *image, void(*f)(Color *c));
void removeAllColors(SDL_Surface *image);
void removeLetters(SDL_Surface *image);
void colorZones(SDL_Surface *image);
void thickenColor(SDL_Surface *surface, Color *c);
Uint32 findAverageColor(SDL_Surface *image);
void setMonochromatic(SDL_Surface *image, Color *c);

Uint32 distanceToColorHSV(Color *c, Color *c2);
Uint32 distanceToColor(Color *c, Color *c2);

void invert(Color *c);
void grayscale(Color *c);
void stepColoring(Color *c);
void enhanceBlack(Color *c);
void removeGreen(Color *c);
void keepTopoLine(Color *c);
void keepTopoLineHSV(Color *c);
void cleanGray(Color *c);
void setMonochromatic(SDL_Surface *image, Color *c);
