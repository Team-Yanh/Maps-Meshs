#pragma once

#include <SDL/SDL.h> // Uint8 type is from SDL
#include <math.h>
#include "imageFilter.h"

typedef struct HSV
{
    float h;
    float s;
    float v;
}HSV;

HSV *RGBToHSV(Uint8 r, Uint8 g, Uint8 b);
Uint32 distanceToColorHSV(Uint8 r, Uint8 g, Uint8 b,
        Uint8 r2, Uint8 g2, Uint8 b2 );
Uint32 distanceToColor(Uint8 r, Uint8 g, Uint8 b,
        Uint8 r2, Uint8 g2, Uint8 b2 );
void invert(Uint8 *r, Uint8 *g, Uint8 *b);
void grayscale(Uint8 *r, Uint8 *g, Uint8 *b);
void stepColoring(Uint8 *r, Uint8 *g, Uint8 *b);
void enhanceBlack(Uint8 *r, Uint8 *g, Uint8 *b);
void removeGreen(Uint8 *r, Uint8 *g, Uint8 *b);
void keepTopoLine(Uint8 *r, Uint8 *g, Uint8 *b);
void keepTopoLineHSV(Uint8 *r, Uint8 *g, Uint8 *b);
void cleanGray(Uint8 *r, Uint8 *g, Uint8 *b);
void setMonochromatic(SDL_Surface *image, Uint32 pixel);
