#pragma once

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <math.h>
#include "queue.h"
#include "imageFilter.h"


Uint32 distanceToColorHSV(Color *c, Color *c2);
Uint32 distanceToColor(Color *c, Color *c2);

void invert(Color *c);
void grayscale(Color *c);
void stepColoring(Color *c);
void enhanceBlack(Color *c);
void thickenColor(SDL_Surface *surface, Color *c);
void keepTopoLine(Color *c);
void keepTopoLineHSV(SDL_Surface *image, Color *topoColor);
void setMonochromatic(SDL_Surface *image, Color *c);
void blur(SDL_Surface **image, int blurLevel);
float *heightList(SDL_Surface *image, int nbRows, int nbCols);

void replaceColor(SDL_Surface *image, Color *c1, Color *c2);
