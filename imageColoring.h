#pragma once

#include "imageFilter.h"
#include "imageUtils.h"

int colorZoneBFS(SDL_Surface *image, Color *c, int x, int y);
void colorCircles(SDL_Surface *image);
void colorAllZonesFromCircles(SDL_Surface *image);

void colorZoneDFS(SDL_Surface *image, Color *c, int x, int y);
int colorZoneDFSCount(SDL_Surface *image, Color *c, int x, int y);
void colorAllZonesNaive(SDL_Surface *image);