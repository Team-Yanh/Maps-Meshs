#pragma once

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <math.h>
#include "queue.h"
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

void forEachPixel(SDL_Surface *image, void(*f)(Color *c));
void removeAllColors(SDL_Surface *image);
void colorZoneDFS(SDL_Surface *image, Color *c, int x, int y);
void colorCircles(SDL_Surface *image);
int colorZoneBFS(SDL_Surface *image, Color *c, int x, int y);
int colorZoneDFSCount(SDL_Surface *image, Color *c, int x, int y);

void colorAllZones(SDL_Surface *image);
int isValidCell(SDL_Surface *surface, int x, int y);
int isSameColor(Color *c1, Color *c2);
int isValidNeighbour(SDL_Surface *image, int x, int y);
void replaceColor(SDL_Surface *image, Color *c1, Color *c2);
void thickenColor(SDL_Surface *surface, Color *c);
void setMonochromatic(SDL_Surface *image, Color *c);

Uint32 distanceToColorHSV(Color *c, Color *c2);
Uint32 distanceToColor(Color *c, Color *c2);

void invert(Color *c);
void grayscale(Color *c);
void stepColoring(Color *c);
void enhanceBlack(Color *c);
void keepTopoLine(Color *c);
void keepTopoLineHSV(Color *c);
