#pragma once

#include "imageFilter.h"
#include "imageUtils.h"

int colorZoneBFS(SDL_Surface *image, Color *c, int x, int y);
int colorCircles(SDL_Surface *image); // return 0 if no circles
int colorRivers(SDL_Surface *image); // return 0 if no river
int colorAllZonesFromCircles(SDL_Surface *image); // return nbcolor used

void normalize(SDL_Surface *image, int nbColors);
void averageImages(SDL_Surface *imageSource, SDL_Surface *imageDest);
void drawHLine(SDL_Surface *image, Color *c, int x, int y, int x2, int y2);
SDL_Surface *addHBorders(SDL_Surface *image, int borderWidth);
SDL_Surface *addVBorders(SDL_Surface *image, int borderHeight);

void colorZoneDFS(SDL_Surface *image, Color *c, int x, int y);
int colorZoneDFSCount(SDL_Surface *image, Color *c, int x, int y);
void colorAllZonesNaive(SDL_Surface *image);
