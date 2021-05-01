#pragma once

#include "imageFilter.h"
#include "imageUtils.h"

int colorZoneBFS(SDL_Surface *image, Color *c, int x, int y);
int colorCircles(SDL_Surface *image); // return 0 if no circles
int colorAllZonesFromCircles(SDL_Surface *image); // return nbcolor used

void normalize(SDL_Surface *image, int nbColors);

void colorZoneDFS(SDL_Surface *image, Color *c, int x, int y);
int colorZoneDFSCount(SDL_Surface *image, Color *c, int x, int y);
void colorAllZonesNaive(SDL_Surface *image);
