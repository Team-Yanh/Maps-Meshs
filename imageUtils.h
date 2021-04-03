#pragma once

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <math.h>
#include "queue.h"
#include "imageFilter.h"

void thickenColor(SDL_Surface *surface, Color *c);

Uint32 distanceToColorHSV(Color *c, Color *c2);
Uint32 distanceToColor(Color *c, Color *c2);

void invert(Color *c);
void grayscale(Color *c);
void stepColoring(Color *c);
void enhanceBlack(Color *c);
void keepTopoLine(Color *c);
void keepTopoLineHSV(Color *c);
void setMonochromatic(SDL_Surface *image, Color *c);

void replaceColor(SDL_Surface *image, Color *c1, Color *c2);